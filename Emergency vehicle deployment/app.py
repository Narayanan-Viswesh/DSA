from flask import Flask, request, jsonify, render_template
import subprocess
import json

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/deploy', methods=['POST'])
def deploy():
    data = request.get_json()
    emergency_type = data['type']
    location = data['location']

    try:
        result = subprocess.run(["./emergency", emergency_type, location], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({"status": "error", "message": "Invalid zone or internal error."})

        # Try parsing output as JSON manually from C-style output
        output = result.stdout.strip()
        try:
            # convert valid JSON string
            json_start = output.find('{')
            json_data = json.loads(output[json_start:])
            return jsonify(json_data)
        except json.JSONDecodeError:
            return jsonify({"status": "error", "message": "Failed to parse C output."})

    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})

if __name__ == '__main__':
    app.run(debug=True)
