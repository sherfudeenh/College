class Solution1 {
    public int findComplement(int num) {

        int ans = 0;
        int place = 1;

        while (num > 0) {

            int bit = num % 2;

            if (bit == 0)
                ans = ans + place;

            place = place * 2;
            num = num / 2;
        }

        return ans;
    }
}