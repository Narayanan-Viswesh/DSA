# 🚨 Emergency Vehicle Deployment System

A full-stack simulation system to optimize emergency vehicle routing using **Dijkstra's Algorithm**, implemented with a C backend, Flask API, and a modern HTML/CSS/JavaScript frontend.

---

## 📌 Problem Statement

Urban emergency response is often delayed due to traffic congestion and poor routing. This project simulates a solution by calculating the most efficient route for emergency vehicles based on a zone-based city layout.

---

## 🧠 Core Concepts

- **Graph Theory**: City is modeled as a graph with zones as nodes.
- **Dijkstra’s Algorithm**: Finds the shortest path between source and destination.
- **Adjacency Matrix**: Stores travel time between zones.
- **Modular Stack**: C backend, Flask middleware, modern web frontend.

---

## 🛠️ Tech Stack

| Layer     | Technology         | Description                                |
|----------|--------------------|--------------------------------------------|
| Backend  | C                  | Core logic for Dijkstra and data storage   |
| API      | Python (Flask)     | Middleware between frontend and backend    |
| Frontend | HTML, CSS, JS      | User interface for zone selection and result display |
| Storage  | File I/O (optional upgrade: SQLite) | Lightweight persistence |

---

## 🚀 Features

- Dropdown selection of emergency type and zones
- Real-time shortest path computation
- Human-readable output (path, time, vehicle type)
- Fast backend performance (< 250 ms total response time)
- Easily scalable to larger graphs or real-world deployment

---

## 🖥️ UI Preview

- Emergency type and zone selectors
- Submit button for request
- Results showing:
  - Dispatched vehicle
  - Source and destination zones
  - Estimated travel time
  - Full route as zone names

---

## 🧪 Sample Input/Output

**Input (JSON):**
```json
{
  "type": "Ambulance",
  "location": "Zone F"
}
```

**Output (JSON):**
```json
{
  "status": "success",
  "type": "Ambulance",
  "source_zone": "Tech Park",
  "destination_zone": "Zone F",
  "travel_time": 12,
  "timestamp": "2025-04-24 18:00:00",
  "path": ["Tech Park", "Zone D", "Zone F"]
}
```

---

## 🧩 Project Structure

```
📁 project-root/
├── backend.c             # C backend with Dijkstra’s algorithm
├── app.py                # Flask middleware
├── templates/
│   └── index.html        # Web UI with dropdowns and display area
├── static/               # CSS or JS files if any
├── emergency_db.txt      # File-based log of requests
├── README.md             # You're here!
```

---

## 🧬 How It Works

1. **User** selects emergency type and zone via frontend.
2. **Flask API** sends this to the **C backend**.
3. **C code** computes the shortest path using Dijkstra.
4. **Result** is returned and displayed with route, vehicle, and time.

---

## 🧠 Future Scope

- ✅ Real-time traffic integration via Google Maps API
- ✅ Scalable to 50+ zones
- ⏳ Vehicle availability logic
- ⏳ Multi-user/concurrent requests support
- ⏳ Dynamic graph configuration (via DB or UI)
- ⏳ Mobile App extension (Flutter/React Native)

---

## 📚 References

- [CLRS Algorithms - MIT Press](https://mitpress.mit.edu/books/introduction-algorithms)
- [GeeksforGeeks - Dijkstra](https://www.geeksforgeeks.org/dijkstra-algorithm-graph-data-structure/)
- Python, C Programming, Flask Documentation

---

## 🤝 Contributions

Feel free to fork the repo and submit pull requests to enhance performance, UI, or add new features!

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

