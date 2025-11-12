# from flask import Flask, request, jsonify, send_from_directory
# import subprocess
# import os

# app = Flask(__name__)

# UPLOAD_DIR = "uploads"
# os.makedirs(UPLOAD_DIR, exist_ok=True)

# # Serve your index.html file when visiting http://127.0.0.1:5000
# @app.route("/")
# def home():
#     return send_from_directory(".", "index.html")

# # Handle form submission and run the selected LBP executable
# @app.route("/run-lbp", methods=["POST"])
# def run_lbp():
#     file = request.files["file"]
#     mode = request.form["mode"]

#     file_path = os.path.join(UPLOAD_DIR, file.filename)
#     file.save(file_path)

#     exe_path = r"C:\Parallel-Computing-Projects-master\Project3\build"
#     exe_name = "lbp_seq.exe" if mode == "seq" else "lbp_omp.exe"
#     full_exe = os.path.join(exe_path, exe_name)

#     try:
#         # Run the program and capture its output
#         result = subprocess.run(
#             [full_exe],
#             capture_output=True,
#             text=True,
#             timeout=60
#         )

#         output = result.stdout.strip() or result.stderr.strip()
#         return jsonify({"output": output})

#     except Exception as e:
#         return jsonify({"output": f"Error: {str(e)}"})

# if __name__ == "__main__":
#     app.run(debug=True)



from flask import Flask, request, jsonify, send_from_directory
import subprocess, os, re

app = Flask(__name__)

UPLOAD_DIR = "uploads"
os.makedirs(UPLOAD_DIR, exist_ok=True)

@app.route("/")
def home():
    return send_from_directory(".", "index.html")

@app.route("/run-lbp", methods=["POST"])
def run_lbp():
    file = request.files["file"]
    file_path = os.path.join(UPLOAD_DIR, file.filename)
    file.save(file_path)

    exe_dir = r"C:\Parallel-Computing-Projects-master\Project3\build"
    seq_exe = os.path.join(exe_dir, "lbp_seq.exe")
    omp_exe = os.path.join(exe_dir, "lbp_omp.exe")

    def run_and_extract(exe):
        result = subprocess.run([exe], capture_output=True, text=True)
        time_match = re.search(r"(\d+\.\d+)\s*seconds", result.stdout)
        time_taken = time_match.group(1) if time_match else "N/A"
        return {"output": result.stdout.strip(), "time": time_taken}

    sequential = run_and_extract(seq_exe)
    parallel = run_and_extract(omp_exe)

    return jsonify({
        "sequential": sequential,
        "parallel": parallel
    })

if __name__ == "__main__":
    app.run(debug=True)

