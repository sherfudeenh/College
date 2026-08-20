class Busroutes {
    public int numBusesToDestination(int[][] routes, int S, int T) {

        if (S == T)
            return 0;

        int n = routes.length;

        // stop -> buses that contain this stop
        HashMap<Integer, HashSet<Integer>> to_routes = new HashMap<>();

        for (int i = 0; i < n; i++) {
            for (int stop : routes[i]) {

                if (!to_routes.containsKey(stop)) {
                    to_routes.put(stop, new HashSet<Integer>());
                }

                to_routes.get(stop).add(i);
            }
        }

        // If source stop doesn't exist in any route
        if (!to_routes.containsKey(S))
            return -1;

        Queue<int[]> bfs = new ArrayDeque<>();

        // {current stop, buses taken}
        bfs.offer(new int[]{S, 0});

        // Stops already visited
        HashSet<Integer> seen = new HashSet<>();
        seen.add(S);

        // Buses already used
        boolean[] seen_routes = new boolean[n];

        while (!bfs.isEmpty()) {

            int stop = bfs.peek()[0];
            int bus = bfs.peek()[1];

            bfs.poll();

            // Reached destination
            if (stop == T)
                return bus;

            // Get buses passing through this stop
            HashSet<Integer> buses = to_routes.get(stop);

            if (buses == null)
                continue;

            for (int i : buses) {

                // Already used this bus
                if (seen_routes[i])
                    continue;

                // Visit all stops of this bus
                for (int nextStop : routes[i]) {

                    if (!seen.contains(nextStop)) {

                        seen.add(nextStop);

                        // Taking this bus increases bus count by 1
                        bfs.offer(new int[]{nextStop, bus + 1});
                    }
                }

                seen_routes[i] = true;
            }
        }

        return -1;
    }
}
