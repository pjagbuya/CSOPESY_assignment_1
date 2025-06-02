import os

def extract_frame_region(source_file, target_file, frame_height=95, start_line=17, end_line=66):
    """
    Extracts lines 17–66 from each frame (of height 95 lines) in a source file
    and writes them into a target file.
    
    Args:
        source_file (str): Path to the input file.
        target_file (str): Path to the output file.
        frame_height (int): Number of lines per frame (default 95).
        start_line (int): Starting line to extract from each frame (1-based).
        end_line (int): Ending line to extract from each frame (inclusive, 1-based).
    """
    try:
        with open(source_file, 'r') as src:
            lines = [line.rstrip('\n') for line in src]  # remove trailing newlines

        total_lines = len(lines)
        frame_count = total_lines // (frame_height + 1)  # +1 for the blank line separator

        with open(target_file, 'w') as tgt:
            for i in range(frame_count):
                frame_start = i * (frame_height + 1)  # starting index of the frame
                frame_lines = lines[frame_start:frame_start + frame_height]

                # Extract lines 17 to 66 (convert to 0-based index)
                extracted = frame_lines[start_line - 1:end_line]
                tgt.write('\n'.join(extracted) + '\n\n')  # separate frames with a blank line

        print(f"Successfully extracted lines {start_line}-{end_line} from {frame_count} frames.")

    except Exception as e:
        print(f"Error: {e}")

        

# Get the folder where the script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct full paths
source_file = os.path.join(script_dir, "teto.txt")
target_file = os.path.join(script_dir, "output.txt")

# Then call the function
extract_frame_region(source_file, target_file)

