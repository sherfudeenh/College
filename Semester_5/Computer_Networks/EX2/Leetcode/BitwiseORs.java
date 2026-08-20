import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class BitwiseORs {
    public int subarrayBitwiseORs(int[] arr) {
        // Keeps track of all distinct OR values found globally
        HashSet<Integer> totalDistinctOrs = new HashSet<>();
        
        // Simulates the set of OR results ending at the previous and current index
        List<Integer> rollingOrs = new ArrayList<>();
        
        int prevStartIndex = 0;
        
        for (int num : arr) {
            int currentSize = rollingOrs.size();
            rollingOrs.add(num);
            
            // Generate new OR combinations using the previous window elements
            for (int i = prevStartIndex; i < currentSize; i++) {
                int nextOrValue = rollingOrs.get(i) | num;
                
                // Only insert if it changes the current state to prevent duplicates
                if (rollingOrs.get(rollingOrs.size() - 1) != nextOrValue) {
                    rollingOrs.add(nextOrValue);
                }
            }
            
            // Advance the window tracking the previous iteration's results
            prevStartIndex = currentSize;
        }
        
        // Deduplicate the entire rolling list at the end for the final count
        totalDistinctOrs.addAll(rollingOrs);
        return totalDistinctOrs.size();
    }
}
