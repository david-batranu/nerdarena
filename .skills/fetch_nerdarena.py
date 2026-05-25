#!/usr/bin/env python3
import sys
import os
import urllib.request
import re
from html.parser import HTMLParser

class NerdArenaParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.in_wiki_block = False
        self.wiki_block_depth = 0
        self.wiki_html = []
        self.time_limit = "0.15s"
        self.memory_limit = "512KB"
        self.input_file = "treicc.in"
        self.output_file = "treicc.out"
        self.current_tag = ""

    def handle_starttag(self, tag, attrs):
        self.current_tag = tag
        attrs_dict = dict(attrs)
        
        # Check for wiki_text_block class
        if 'class' in attrs_dict and 'wiki_text_block' in attrs_dict['class']:
            self.in_wiki_block = True
            self.wiki_block_depth = 0
        
        if self.in_wiki_block:
            self.wiki_block_depth += 1
            # Reconstruct starting tag inside wiki block
            attr_str = "".join([f' {k}="{v}"' for k, v in attrs])
            self.wiki_html.append(f"<{tag}{attr_str}>")

    def handle_endtag(self, tag):
        if self.in_wiki_block:
            self.wiki_html.append(f"</{tag}>")
            self.wiki_block_depth -= 1
            if self.wiki_block_depth == 0:
                self.in_wiki_block = False
        self.current_tag = ""

    def handle_data(self, data):
        if self.in_wiki_block:
            self.wiki_html.append(data)

def clean_html_to_markdown(html_content):
    # Extremely basic and clean converter from HTML to Markdown for competitive programming text
    text = html_content
    # Remove task-header table since we will parse it separately
    text = re.sub(r'<table class="task-header">.*?</table>', '', text, flags=re.DOTALL)
    # Remove attachments banner
    text = re.sub(r'<div class="open-task-banner">.*?</div>', '', text, flags=re.DOTALL)
    # Remove stats links
    text = re.sub(r'<p class="task-title-links">.*?</p>', '', text, flags=re.DOTALL)
    # Remove login messages
    text = re.sub(r'<div class="macroMessage">.*?</div>', '', text, flags=re.DOTALL)
    # Remove hint/algorithm tags at the end
    text = re.sub(r'<h3>\s*Indicii de rezolvare.*?</ul>\s*</div>', '', text, flags=re.DOTALL)
    text = re.sub(r'<div[^>]*>\s*<h3>\s*Indicii de rezolvare.*?</div>', '', text, flags=re.DOTALL)
    
    # Replace headers
    text = re.sub(r'<h1>(.*?)</h1>', r'# \1\n', text)
    text = re.sub(r'<h2>(.*?)</h2>', r'## \1\n', text)
    text = re.sub(r'<h3>(.*?)</h3>', r'### \1\n', text)
    
    # Replace list items
    text = re.sub(r'<li>(.*?)</li>', r'- \1', text)
    text = re.sub(r'</?ul[^>]*>', '', text)
    
    # Replace bold, italic, var, code
    text = re.sub(r'<strong>(.*?)</strong>', r'**\1**', text)
    text = re.sub(r'<b>(.*?)</b>', r'**\1**', text)
    text = re.sub(r'<var>(.*?)</var>', r'*\1*', text)
    text = re.sub(r'<em>(.*?)</em>', r'*\1*', text)
    text = re.sub(r'<code>(.*?)</code>', r'`\1`', text)
    
    # Replace table to markdown
    # A simple replacement for the example table
    def table_repl(match):
        table_content = match.group(1)
        # Find headers
        headers = re.findall(r'<th>(.*?)</th>', table_content, re.DOTALL)
        # Find rows (excluding headers)
        rows = re.findall(r'<tr>(.*?)</tr>', table_content, re.DOTALL)
        row_data = []
        for r in rows:
            tds = re.findall(r'<td>(.*?)</td>', r, re.DOTALL)
            if tds:
                row_data.append([td.replace('<br />', '\n').strip() for td in tds])
        
        md_table = ""
        if headers:
            headers = [h.strip() for h in headers]
            md_table += "| " + " | ".join(headers) + " |\n"
            md_table += "| " + " | ".join(["---"] * len(headers)) + " |\n"
        
        for rd in row_data:
            # We might have multi-line values, so let's format them simply
            md_table += "| " + " | ".join([v.replace('\n', ' <br> ') for v in rd]) + " |\n"
        return md_table

    text = re.sub(r'<table[^>]*class="example"[^>]*>(.*?)</table>', table_repl, text, flags=re.DOTALL)
    
    # Remove other HTML tags but keep content
    text = re.sub(r'</?p[^>]*>', '\n', text)
    text = re.sub(r'</?div[^>]*>', '\n', text)
    text = re.sub(r'<br\s*/?>', '\n', text)
    
    # Normalize whitespace
    lines = [line.strip() for line in text.split('\n')]
    new_lines = []
    for line in lines:
        if line:
            new_lines.append(line)
        elif not new_lines or new_lines[-1] != "":
            new_lines.append("")
    
    return "\n".join(new_lines).strip()

def main():
    if len(sys.argv) < 3:
        print("Usage: fetch_nerdarena.py <problem_url> <target_file>")
        sys.exit(1)
        
    url = sys.argv[1]
    target_path = sys.argv[2]
    
    print(f"Fetching {url}...")
    req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
    try:
        with urllib.request.urlopen(req) as response:
            html = response.read().decode('utf-8')
    except Exception as e:
        print(f"Error fetching URL: {e}")
        sys.exit(1)
        
    # Extract limits
    time_limit = "0.15s"
    memory_limit = "512KB"
    input_file = "stdin"
    output_file = "stdout"
    
    # Try regex search for simple parsing of limits
    time_match = re.search(r'Timp de execuție pe test.*?<td>\s*([\d\.]+)\s*(sec|s)', html, re.DOTALL | re.IGNORECASE)
    if time_match:
        time_limit = f"{time_match.group(1)}s"
        
    mem_match = re.search(r'Limită de memorie.*?<td>\s*(\d+)\s*(KB|MB)', html, re.DOTALL | re.IGNORECASE)
    if mem_match:
        memory_limit = f"{mem_match.group(1)}{mem_match.group(2)}"
        
    file_match = re.search(r'Fișierul intrare/ieșire.*?<td>\s*([a-zA-Z0-9_\.]+),\s*([a-zA-Z0-9_\.]+)', html, re.DOTALL | re.IGNORECASE)
    if file_match:
        input_file = file_match.group(1).strip()
        output_file = file_match.group(2).strip()
    
    # Parse the main content using HTMLParser
    parser = NerdArenaParser()
    parser.feed(html)
    
    raw_wiki = "".join(parser.wiki_html)
    markdown_content = clean_html_to_markdown(raw_wiki)
    
    # Formulate output file content
    output_lines = [
        f"# Treicc",
        f"",
        f"**Time Limit:** {time_limit}",
        f"**Memory Limit:** {memory_limit}",
        f"**Input File:** `{input_file}`",
        f"**Output File:** `{output_file}`",
        f"",
        markdown_content
    ]
    
    os.makedirs(os.path.dirname(os.path.abspath(target_path)), exist_ok=True)
    with open(target_path, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines) + "\n")
        
    print(f"Saved problem markdown to {target_path}")

if __name__ == "__main__":
    main()
