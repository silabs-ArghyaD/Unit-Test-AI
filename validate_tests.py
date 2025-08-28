#!/usr/bin/env python3
"""
Unit Test Validator using LiteLLM and GPT-4.1
This script validates existing test cases against the validation template.
"""

import json
import os
import sys
import argparse
from pathlib import Path
from litellm import completion

# Configure LiteLLM for your custom proxy
import litellm
litellm.api_base = "https://litellm.silabs.net/v1"  # Your custom proxy endpoint
litellm.set_verbose = True  # Enable verbose logging for debugging

def read_file_content(file_path):
    """Read and return the content of a file."""
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            return file.read()
    except Exception as e:
        print(f"Error reading file {file_path}: {e}")
        return None

def load_validation_template():
    """Load the validation template markdown file."""
    template_path = Path(__file__).parent / "ValidationTemplate.md"
    content = read_file_content(template_path)
    if not content:
        print("Error: Could not load ValidationTemplate.md")
        sys.exit(1)
    return content

def test_api_connection():
    """Test the API connection before running validation"""
    try:
        print("Testing API connection with gpt-4.1 via LiteLLM proxy...")
        response = completion(
            model="gpt-4.1",  # Use GPT-4.1 as requested
            messages=[{"role": "user", "content": "Hello"}],
            max_tokens=5,
            api_key=os.getenv("LITELLM_TOKEN")  # Use your token
        )
        print("✅ API connection successful")
        return True
    except Exception as e:
        print(f"❌ API connection failed: {e}")
        print(f"Error type: {type(e).__name__}")
        if hasattr(e, 'response'):
            print(f"Response: {e.response}")
        return False

def validate_test_file_with_ai(test_file_path, validation_template):
    """Use GPT-4.1 to validate a test file against the template."""
    
    # Read the test file content
    test_content = read_file_content(test_file_path)
    if not test_content:
        return {
            "file": test_file_path,
            "status": "ERROR",
            "error": f"Could not read test file: {test_file_path}"
        }
    
    # Create the prompt for GPT-4.1
    prompt = f"""
You are a unit test validator. Please analyze the following test file against the provided validation template.

VALIDATION TEMPLATE:
{validation_template}

TEST FILE TO VALIDATE:
File: {test_file_path}

```cpp
{test_content}
```

Please provide validation results in the exact format specified in the template. Focus only on the 3 basic requirements:
1. Component Header Inclusion (.cpp file)
2. Test Function Naming (starts with TEST)
3. Test Suite Naming (proper pattern)

Provide a clear PASS/FAIL status and specific recommendations if needed.
"""

    try:
        # Use GPT-4.1 model via your LiteLLM proxy
        model_name = "gpt-4.1"
        
        # Call GPT-4.1 using your LiteLLM proxy
        response = completion(
            model=model_name,
            messages=[
                {
                    "role": "system", 
                    "content": "You are a code validator specializing in C++ unit test validation. Follow the template instructions precisely."
                },
                {
                    "role": "user", 
                    "content": prompt
                }
            ],
            temperature=0.1,  # Low temperature for consistent results
            max_tokens=2000,
            api_key=os.getenv("LITELLM_TOKEN")  # Use your token
        )
        
        validation_result = response.choices[0].message.content
        
        # Parse the status from the response
        status = "UNKNOWN"
        if "Status: PASS" in validation_result or "🎯 Status: PASS" in validation_result:
            status = "PASS"
        elif "Status: FAIL" in validation_result or "🎯 Status: FAIL" in validation_result:
            status = "FAIL"
        
        return {
            "file": test_file_path,
            "status": status,
            "validation_result": validation_result
        }
        
    except Exception as e:
        return {
            "file": test_file_path,
            "status": "ERROR",
            "error": f"AI validation failed: {str(e)}"
        }

def main():
    parser = argparse.ArgumentParser(description="Validate unit test files using AI")
    parser.add_argument("--test-files", help="Path to file containing list of test files to validate")
    parser.add_argument("--mapping-file", help="Path to file_test_mapping.txt to extract relevant test files")
    parser.add_argument("--output", help="Output file for results (default: stdout)")
    
    args = parser.parse_args()
    
    # Check for LiteLLM token
    if not os.getenv("LITELLM_TOKEN"):
        print("Error: LITELLM_TOKEN environment variable is required")
        sys.exit(1)
    
    print("Using GPT-4.1 via LiteLLM proxy for validation...")
    
    # Test API connection first
    if not test_api_connection():
        print("❌ Cannot proceed without API connection")
        print("Please check your LITELLM_TOKEN and proxy configuration")
        sys.exit(1)
    
    # Load validation template
    validation_template = load_validation_template()
    
    # Read test files list
    test_files = []
    
    if args.mapping_file:
        # Extract test files from mapping file
        try:
            with open(args.mapping_file, 'r') as f:
                for line in f:
                    line = line.strip()
                    if ':' in line:
                        modified_file, test_files_str = line.split(':', 1)
                        if test_files_str.strip():
                            # Split by spaces and newlines to get individual test files
                            for test_file in test_files_str.split():
                                test_file = test_file.strip()
                                if test_file and test_file not in test_files:
                                    test_files.append(test_file)
        except Exception as e:
            print(f"Error reading mapping file: {e}")
            sys.exit(1)
    elif args.test_files:
        # Read from regular test files list
        try:
            with open(args.test_files, 'r') as f:
                test_files = [line.strip() for line in f if line.strip()]
        except Exception as e:
            print(f"Error reading test files list: {e}")
            sys.exit(1)
    else:
        print("Error: Either --test-files or --mapping-file must be provided")
        sys.exit(1)
    
    if not test_files:
        print("No test files found to validate")
        return
    
    print(f"Validating {len(test_files)} test files...")
    
    results = []
    for test_file in test_files:
        print(f"Validating: {test_file}")
        result = validate_test_file_with_ai(test_file, validation_template)
        results.append(result)
        
        # Print status
        status_emoji = "✅" if result["status"] == "PASS" else "❌" if result["status"] == "FAIL" else "⚠️"
        print(f"  {status_emoji} {result['status']}")
    
    # Generate summary report
    total_files = len(results)
    passed_files = len([r for r in results if r["status"] == "PASS"])
    failed_files = len([r for r in results if r["status"] == "FAIL"])
    error_files = len([r for r in results if r["status"] == "ERROR"])
    
    # Create detailed report
    report = f"""
# Unit Test Validation Report

## Summary
- **Total Files**: {total_files}
- **Passed**: {passed_files}
- **Failed**: {failed_files}
- **Errors**: {error_files}

## Detailed Results

"""
    
    for result in results:
        report += f"\n---\n\n"
        if result["status"] == "ERROR":
            report += f"## ⚠️ ERROR: {result['file']}\n\n"
            report += f"**Error**: {result.get('error', 'Unknown error')}\n\n"
        else:
            report += result.get('validation_result', f"Status: {result['status']}")
        report += "\n"
    
    # Output results
    if args.output:
        with open(args.output, 'w') as f:
            f.write(report)
        print(f"\nDetailed report saved to: {args.output}")
    else:
        print(report)
    
    # Print final summary
    print(f"\n{'='*50}")
    print(f"VALIDATION SUMMARY")
    print(f"{'='*50}")
    print(f"Total: {total_files} | Passed: {passed_files} | Failed: {failed_files} | Errors: {error_files}")
    
    if failed_files > 0 or error_files > 0:
        print(f"\n❌ Validation completed with issues")
        sys.exit(1)
    else:
        print(f"\n✅ All tests passed validation")

if __name__ == "__main__":
    main()
