# 整理與驗證工具

[課程首頁](../README.md)

| 工具 | 功能 | 相依套件 |
| --- | --- | --- |
| `import_course.py` | 從原課程資料匯入 C／文字／DOT 與 Word 內嵌圖片，建立來源雜湊 | pypdf |
| `inspect_assets.py` | 建立本機聯絡表，供逐張圖片視覺核對 | Pillow |
| `verify_course.py` | 獨立編譯全部 C 程式，執行 18 組代表性檢查 | GCC、Python 標準函式庫 |
| `catalog_course.py` | 依已核對的主題中繼資料生成 FILES／RESULTS 與資料夾索引 | Python 標準函式庫 |
| `check_repository.py` | 檢查 Markdown 連結、圖片完整性、SVG、來源雜湊與禁止上傳的檔案 | Pillow |

建議 Python 3.10 以上；套件可由 `python -m pip install -r tools/requirements.txt` 安裝。工具在 repository 根目錄執行。`verify_course.py` 使用 GCC GNU C11，而非預設假設所有 C 編譯器都相容。

```powershell
python -X utf8 tools/verify_course.py --cc gcc
python -X utf8 tools/catalog_course.py
python -X utf8 tools/check_repository.py
```

匯入需持有原課程資料夾，不能只靠 GitHub 中的檔案重建原 Word／PDF。`import_course.py` 會覆寫相同目的地的匯入檔與來源 manifest；不要對已自行修改的匯入 C 檔直接重跑。它不修改原資料夾，抽出的原文件文字與 contact sheets 放在忽略的 `.local/`。

圖片 caption 以人工核對的主題群組整理，不是 OCR 的逐字轉錄，也不是程式自動推斷結果正確。生成的索引可以重建，詳細作業主 README 與分析文件則為手工整理。
