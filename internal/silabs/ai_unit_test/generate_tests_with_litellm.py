#!/usr/bin/env python3
"""
Unit Test Generator using LiteLLM (GPT-4.1)
Generates a C++ unit test file for a given .cpp source using UnitTestTemplate.md.
Compatible with Python 3.8+ (uses typing.Optional instead of PEP 604 unions).
"""

import os
import sys
import re
import argparse
import requests
from pathlib import Path
from typing import Optional

LITELLM_API = "https://litellm.silabs.net/chat/completions"

def read_text(path: Path) -> Optional[str]:
    try:
        return path.read_text(encoding="utf-8")
    except Exception as e:
        print(f"❌ Error reading '{path}': {e}")
        return None

def resolve_template_path(user_supplied: Optional[str]) -> Path:
    if user_supplied:
        return Path(user_supplied).expanduser().resolve()
    # default to UnitTestTemplate.md in the same directory as this script
    return (Path(__file__).parent / "UnitTestTemplate.md").resolve()

def compute_output_path(src_path: Path, out_dir: Optional[str]) -> Path:
    stem = src_path.stem
    default_name = f"Test_{stem}.cpp"
    if out_dir:
        return (Path(out_dir).expanduser().resolve() / default_name)
    # put next to source by default
    return (src_path.parent / default_name).resolve()

def parse_coverage_report(coverage_path: Path, source_filename: str) -> Optional[str]:
    """Parse coverage report and extract information for the specific source file."""
    try:
        coverage_content = read_text(coverage_path)
        if not coverage_content:
            return None
        
        lines = coverage_content.split('\n')
        coverage_info = []
        in_file_section = False
        total_coverage = None
        
        # Extract overall coverage summary
        for line in lines:
            if 'TOTAL' in line and '%' in line:
                # Extract total coverage percentage
                match = re.search(r'(\d+\.?\d*)%', line)
                if match:
                    total_coverage = match.group(1)
            
            # Look for the specific source file
            if source_filename in line:
                parts = line.split()
                if len(parts) >= 4:
                    # Try to parse coverage line format: File Lines Exec Cover Missing
                    for i, part in enumerate(parts):
                        if '%' in part:
                            coverage_info.append(f"File coverage: {part}")
                            if i > 0 and parts[i-1].isdigit():
                                coverage_info.append(f"Lines executed: {parts[i-1]} out of total lines")
                            break
        
        result = []
        if total_coverage:
            result.append(f"Overall project coverage: {total_coverage}%")
        
        if coverage_info:
            result.extend(coverage_info)
        elif total_coverage:
            result.append(f"Specific file coverage not found, but overall coverage is {total_coverage}%")
        
        return '\n'.join(result) if result else None
        
    except Exception as e:
        print(f"❌ Error parsing coverage file: {e}")
        return None

def build_prompt(unit_test_template: str, source_path: Path, source_code: str, coverage_info: Optional[str] = None) -> str:
    base_prompt = f"""You are generating a C++ unit test file for the provided component implementation.
Follow the style and requirements from the UNIT TEST TEMPLATE below.

UNIT TEST TEMPLATE (authoritative):
{unit_test_template}

SOURCE FILE UNDER TEST: {source_path}

SOURCE IMPLEMENTATION CONTENT:
```cpp
{source_code}
```"""

    if coverage_info:
        base_prompt += f"""

CURRENT CODE COVERAGE INFORMATION:
{coverage_info}

COVERAGE IMPROVEMENT GUIDANCE:
- Focus on improving test coverage for areas that are currently under-tested
- Add tests for edge cases, error conditions, and boundary values
- Ensure comprehensive coverage of all public methods and functions which are not covered by existing tests
- Consider negative test cases and exception handling"""

    base_prompt += """

RESPONSE RULES (MUST FOLLOW):
- Return ONLY the contents of a single valid C++ test file.
- No markdown, no backticks, no commentary, no headers, no explanations.
- Begin with required #include lines.
- Use Pigweed/googletest macros as indicated by the template.
- Name the test suite as instructed by the template (e.g., <ComponentName>Validation or <ComponentName>Test).
- Ensure it compiles assuming the component is located as shown in the include paths.
- Include at least one positive and one negative test if meaningful.
- Keep it concise and focused on template conformance."""

    if coverage_info:
        base_prompt += """
- Prioritize test cases that will improve code coverage based on the coverage information provided."""

    return base_prompt

def extract_cpp(raw: str) -> str:
    """Extract raw C++ from a model response that might include markdown fences."""
    if not raw:
        return ""
    text = raw.strip()

    # If fenced code block exists, capture the first block
    fence_match = re.search(r"```(?:cpp|c\+\+)?\s*(.*?)```", text, flags=re.S | re.I)
    if fence_match:
        text = fence_match.group(1).strip()

    # Remove any stray triple backticks or language tags
    text = re.sub(r"^```.*?$", "", text, flags=re.M)
    text = text.replace("```", "")
    return text.strip()

def call_litellm_gpt41(prompt: str, token: str) -> str:
    headers = {
        "accept": "application/json",
        "authorization": f"Bearer {token}",
        "content-type": "application/json",
    }
    payload = {
        "model": "gpt-4.1",
        "messages": [
            {"role": "system", "content": "You are a senior C++ unit test generator. Output only raw C++ code."},
            {"role": "user", "content": prompt},
        ],
        "temperature": 0.2,
        "max_tokens": 2600,
    }
    resp = requests.post(LITELLM_API, headers=headers, json=payload, timeout=120)
    resp.raise_for_status()
    data = resp.json()
    return data["choices"][0]["message"]["content"]

def main():
    parser = argparse.ArgumentParser(description="Generate C++ test file from UnitTestTemplate.md using LiteLLM GPT-4.1")
    parser.add_argument("source", help="Path to the component .cpp file to create tests for")
    parser.add_argument("--template", help="Path to UnitTestTemplate.md (defaults to file next to this script)")
    parser.add_argument("--coverage", help="Path to coverage report text file to analyze and improve coverage")
    parser.add_argument("--out-dir", help="Directory to write the generated test (defaults next to source)")
    parser.add_argument("--overwrite", action="store_true", help="Allow overwriting the output file")
    args = parser.parse_args()

    token = "sk-gA5GLGokHyRukblqKL0Cxw"
    if not token:
        print("❌ LITELLM_TOKEN environment variable required")
        sys.exit(1)

    src_path = Path(args.source).expanduser().resolve()
    if not src_path.exists():
        print(f"❌ Source file not found: {src_path}")
        sys.exit(1)
    source_code = read_text(src_path)
    if source_code is None:
        sys.exit(1)

    template_path = resolve_template_path(args.template)
    if not template_path.exists():
        print(f"❌ UnitTestTemplate.md not found at: {template_path}")
        sys.exit(1)
    unit_test_template = read_text(template_path)
    if unit_test_template is None:
        sys.exit(1)

    out_path = compute_output_path(src_path, args.out_dir)
    if out_path.exists() and not args.overwrite:
        print(f"❌ Output already exists: {out_path}. Use --overwrite to replace it.")
        sys.exit(1)

    # Parse coverage information if provided
    coverage_info = None
    if args.coverage:
        coverage_path = Path(args.coverage).expanduser().resolve()
        if not coverage_path.exists():
            print(f"❌ Coverage file not found: {coverage_path}")
            sys.exit(1)
        
        source_filename = src_path.name
        coverage_info = parse_coverage_report(coverage_path, source_filename)
        if coverage_info:
            print(f"📊 Found coverage information for {source_filename}")
        else:
            print(f"⚠️ No specific coverage info found for {source_filename}, using general coverage data")

    print(f"🧪 Generating test for: {src_path}")
    print(f"📄 Using template: {template_path}")
    if args.coverage:
        print(f"📊 Using coverage data: {coverage_path}")
    print(f"📝 Output file: {out_path}")

    try:
        raw = call_litellm_gpt41(
            build_prompt(unit_test_template, src_path, source_code, coverage_info),
            token=token
        )
        cpp_test_code = extract_cpp(raw)
        if not cpp_test_code:
            print("❌ The model did not return any C++ code.")
            sys.exit(1)
    except Exception as e:
        print(f"❌ Generation failed: {e}")
        sys.exit(1)

    try:
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(cpp_test_code, encoding="utf-8")
    except Exception as e:
        print(f"❌ Could not write output file: {e}")
        sys.exit(1)

    print(f"✅ Test created: {out_path} ({len(cpp_test_code.splitlines())} lines)")

if __name__ == "__main__":
    main()
