#!/usr/bin/env python3
"""按补充资料清单下载 Release 附件，恢复原名并校验 SHA-256。"""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import urllib.request


def digest(path):
    result = hashlib.sha256()
    with path.open('rb') as stream:
        while block := stream.read(8 * 1024 * 1024):
            result.update(block)
    return result.hexdigest()


def download(entry, output):
    target = (output / entry['destination']).resolve()
    if not target.is_relative_to(output.resolve()):
        raise ValueError('清单中的文件路径越界')
    if target.exists():
        if target.stat().st_size == entry['size'] and digest(target) == entry['sha256']:
            print('已校验，跳过：', entry['destination'])
            return
        raise ValueError('目标文件已存在但内容不同，请先移走：' + str(target))
    target.parent.mkdir(parents=True, exist_ok=True)
    temporary = target.with_name(target.name + '.part')
    offset = temporary.stat().st_size if temporary.exists() else 0
    if offset > entry['size']:
        raise ValueError('续传文件大小异常：' + str(temporary))
    parts = entry.get('download_parts') or [{'url': entry['download_url'], 'size': entry['size']}]
    if sum(part['size'] for part in parts) != entry['size']:
        raise ValueError('清单中的分卷大小不一致')
    part_start = 0
    for part in parts:
        if offset >= part_start + part['size']:
            part_start += part['size']
            continue
        part_offset = offset - part_start
        headers = {'User-Agent': 'smart-car-resource-downloader'}
        if part_offset:
            headers['Range'] = f'bytes={part_offset}-'
        request = urllib.request.Request(part['url'], headers=headers)
        with urllib.request.urlopen(request, timeout=120) as response:
            status = response.status
            if status == 206:
                content_range = response.headers.get('Content-Range', '')
                if not content_range.startswith(f'bytes {part_offset}-'):
                    raise ValueError('服务器返回了错误的续传位置')
            elif status == 200:
                if part_offset:
                    with temporary.open('r+b') as stream:
                        stream.truncate(part_start)
            else:
                raise ValueError(f'下载响应异常：HTTP {status}')
            with temporary.open('ab') as stream:
                while block := response.read(8 * 1024 * 1024):
                    stream.write(block)
        part_start += part['size']
        offset = temporary.stat().st_size
        if offset != part_start:
            raise ValueError('下载分卷大小不符：' + str(temporary))
    if temporary.stat().st_size != entry['size'] or digest(temporary) != entry['sha256']:
        raise ValueError('下载校验失败，请移走 .part 文件后重试：' + str(temporary))
    temporary.replace(target)
    print('下载并校验完成：', entry['destination'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    selection = parser.add_mutually_exclusive_group()
    selection.add_argument('--list', action='store_true', help='列出可下载附件')
    selection.add_argument('--all', action='store_true', help='下载全部 Release 附件')
    selection.add_argument('--match', help='只下载路径中包含指定文字的附件')
    parser.add_argument('--output', type=Path, default=Path(__file__).resolve().parent / 'release-downloads', help='输出目录')
    args = parser.parse_args()
    manifest = Path(__file__).resolve().with_name('补充资料清单.json')
    entries = [e for e in json.loads(manifest.read_text(encoding='utf-8'))['files'] if e['action'] == 'release']
    if args.match:
        entries = [e for e in entries if args.match.casefold() in e['destination'].casefold()]
    if args.list or not (args.all or args.match):
        for entry in entries:
            print(f"{entry['size'] / 1024**2:8.1f} MiB  {entry['destination']}")
        print(f"共 {len(entries)} 个附件，{sum(e['size'] for e in entries) / 1024**3:.2f} GiB")
        return
    for entry in entries:
        print('开始下载：', entry['destination'], flush=True)
        download(entry, args.output)
    print('全部所选附件下载完成。')


if __name__ == '__main__':
    try:
        main()
    except Exception as error:
        print('错误：', error, file=sys.stderr)
        sys.exit(1)
