const stations = [
    "Shahdara",
    "Niazi Chowk",
    "Timber Market",
    "Azadi Chowk",
    "Bhatti Chowk",
    "Katchery",
    "Civil Secretariat",
    "MAO College",
    "Janazgah",
    "Qartaba Chowk",
    "Shama",
    "Ichhra",
    "Canal",
    "Qaddafi Stadium",
    "Kalma Chowk",
    "Model Town",
    "Naseerabad",
    "Ittefaq Hospital",
    "Qainchi",
    "Ghazi Chowk",
    "Chungi Amar Sidhu",
    "Kamahan",
    "Atari Saroba",
    "Nishtar Colony",
    "Youhanabad",
    "Dulu Khurd",
    "Gajjumata"
];

const edges = [
    ["Shahdara", "Niazi Chowk", 2200],
    ["Niazi Chowk", "Timber Market", 560],
    ["Timber Market", "Azadi Chowk", 1100],
    ["Azadi Chowk", "Bhatti Chowk", 1200],
    ["Bhatti Chowk", "Katchery", 630],
    ["Katchery", "Civil Secretariat", 700],
    ["Civil Secretariat", "MAO College", 900],
    ["MAO College", "Janazgah", 1100],
    ["Janazgah", "Qartaba Chowk", 700],
    ["Qartaba Chowk", "Shama", 1200],
    ["Shama", "Ichhra", 830],
    ["Ichhra", "Canal", 1700],
    ["Canal", "Qaddafi Stadium", 730],
    ["Qaddafi Stadium", "Kalma Chowk", 1000],
    ["Kalma Chowk", "Model Town", 700],
    ["Model Town", "Naseerabad", 1300],
    ["Naseerabad", "Ittefaq Hospital", 930],
    ["Ittefaq Hospital", "Qainchi", 1800],
    ["Qainchi", "Ghazi Chowk", 900],
    ["Ghazi Chowk", "Chungi Amar Sidhu", 830],
    ["Chungi Amar Sidhu", "Kamahan", 1100],
    ["Kamahan", "Atari Saroba", 1200],
    ["Atari Saroba", "Nishtar Colony", 1200],
    ["Nishtar Colony", "Youhanabad", 830],
    ["Youhanabad", "Dulu Khurd", 830],
    ["Dulu Khurd", "Gajjumata", 830]
];

let mode = "distance";

const graph = new Map();
stations.forEach((station) => graph.set(station, []));
edges.forEach(([from, to, weight]) => {
    graph.get(from).push({ station: to, weight });
    graph.get(to).push({ station: from, weight });
});

const fromSelect = document.getElementById("fromStation");
const toSelect = document.getElementById("toStation");
const distanceMode = document.getElementById("distanceMode");
const stopsMode = document.getElementById("stopsMode");
const findRoute = document.getElementById("findRoute");
const routeLine = document.getElementById("routeLine");
const routeSteps = document.getElementById("routeSteps");
const segmentTable = document.getElementById("segmentTable");
const routeTitle = document.getElementById("routeTitle");
const distanceValue = document.getElementById("distanceValue");
const stationValue = document.getElementById("stationValue");

function formatKm(meters) {
    return `${(meters / 1000).toFixed(2)} km`;
}

function fillSelect(select, selectedStation) {
    select.innerHTML = "";
    stations.forEach((station) => {
        const option = document.createElement("option");
        option.value = station;
        option.textContent = station;
        option.selected = station === selectedStation;
        select.appendChild(option);
    });
}

function dijkstra(start, destination) {
    const distance = new Map();
    const parent = new Map();
    const visited = new Set();

    stations.forEach((station) => distance.set(station, Infinity));
    distance.set(start, 0);

    while (visited.size < stations.length) {
        let current = null;
        let bestDistance = Infinity;

        stations.forEach((station) => {
            if (!visited.has(station) && distance.get(station) < bestDistance) {
                current = station;
                bestDistance = distance.get(station);
            }
        });

        if (current === null || current === destination) {
            break;
        }

        visited.add(current);

        graph.get(current).forEach((route) => {
            const nextDistance = distance.get(current) + route.weight;
            if (nextDistance < distance.get(route.station)) {
                distance.set(route.station, nextDistance);
                parent.set(route.station, current);
            }
        });
    }

    return buildPath(start, destination, parent, distance.get(destination));
}

function bfs(start, destination) {
    const queue = [start];
    const visited = new Set([start]);
    const parent = new Map();

    while (queue.length > 0) {
        const current = queue.shift();
        if (current === destination) {
            break;
        }

        graph.get(current).forEach((route) => {
            if (!visited.has(route.station)) {
                visited.add(route.station);
                parent.set(route.station, current);
                queue.push(route.station);
            }
        });
    }

    const result = buildPath(start, destination, parent, visited.has(destination) ? 0 : Infinity);
    result.distance = calculateDistance(result.path);
    return result;
}

function buildPath(start, destination, parent, distance) {
    if (!Number.isFinite(distance)) {
        return { path: [], distance: 0 };
    }

    const path = [];
    let current = destination;

    while (current) {
        path.push(current);
        if (current === start) {
            break;
        }
        current = parent.get(current);
    }

    return { path: path.reverse(), distance };
}

function calculateDistance(path) {
    let total = 0;
    for (let i = 0; i + 1 < path.length; i++) {
        const from = path[i];
        const to = path[i + 1];
        const route = graph.get(from).find((item) => item.station === to);
        total += route ? route.weight : 0;
    }
    return total;
}

function renderRouteLine(path) {
    const active = new Set(path);
    routeLine.innerHTML = "";

    stations.forEach((station) => {
        const node = document.createElement("div");
        node.className = active.has(station) ? "station-node active" : "station-node";
        node.textContent = station;
        routeLine.appendChild(node);
    });
}

function renderRouteDetails(path) {
    routeSteps.innerHTML = "";
    segmentTable.innerHTML = "";

    path.forEach((station) => {
        const item = document.createElement("li");
        item.textContent = station;
        routeSteps.appendChild(item);
    });

    for (let i = 0; i + 1 < path.length; i++) {
        const from = path[i];
        const to = path[i + 1];
        const route = graph.get(from).find((item) => item.station === to);
        const row = document.createElement("div");
        row.className = "segment-row";
        row.innerHTML = `<span>${from} to ${to}</span><strong>${formatKm(route.weight)}</strong>`;
        segmentTable.appendChild(row);
    }
}

function updateRoute() {
    const start = fromSelect.value;
    const destination = toSelect.value;
    const result = mode === "distance" ? dijkstra(start, destination) : bfs(start, destination);

    routeTitle.textContent = `${start} to ${destination}`;
    distanceValue.textContent = formatKm(result.distance);
    stationValue.textContent = result.path.length;
    renderRouteLine(result.path);
    renderRouteDetails(result.path);
}

distanceMode.addEventListener("click", () => {
    mode = "distance";
    distanceMode.classList.add("active");
    stopsMode.classList.remove("active");
    updateRoute();
});

stopsMode.addEventListener("click", () => {
    mode = "stops";
    stopsMode.classList.add("active");
    distanceMode.classList.remove("active");
    updateRoute();
});

findRoute.addEventListener("click", updateRoute);
fromSelect.addEventListener("change", updateRoute);
toSelect.addEventListener("change", updateRoute);

fillSelect(fromSelect, "Shahdara");
fillSelect(toSelect, "Gajjumata");
updateRoute();
