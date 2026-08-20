class Totalhamdist {
    public int totalHammingDistance(int[] nums) {
        int result = 0;
        for (int i = 0; i < 32; i++) {
            int bit = 0;
            for (int num : nums) bit += (num >> i) & 1;
            result += bit * (nums.length - bit);
        }
        return result;
    }
}
