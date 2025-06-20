def convert_frames_to_cpp_array(input_file, output_file, frame_height=50):
    with open(input_file, 'r', encoding='utf-8') as f:
        lines = [line.rstrip('\n') for line in f if line.strip() != '']

    frames = [lines[i:i+frame_height] for i in range(0, len(lines), frame_height)]

    with open(output_file, 'w', encoding='utf-8') as out:
        out.write('std::vector<std::vector<std::string>> frames = {\n')
        for frame in frames:
            out.write('    {\n')
            for line in frame:
                escaped = line.replace('\\', '\\\\').replace('"', '\\"')
                out.write(f'        "{escaped}",\n')
            out.write('    },\n')
        out.write('};\n')

    print(f"Converted {len(frames)} frames to C++ array in '{output_file}'.")

# Example usage
convert_frames_to_cpp_array('teto_cut.txt', 'frames_output.txt', frame_height=50)
