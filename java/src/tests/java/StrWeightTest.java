import dag.StrWeight;
import dag.WeightComparison;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class StrWeightTest {

    @Test
    public void lessThanTests() {
        StrWeight w1 = new StrWeight("a");

        assertEquals(WeightComparison.EQUAL, w1.compare(new StrWeight("a")));

        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("ab")));
        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("ba")));
        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("baa")));
        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("abc")));

    }

    @Test
    public void diffLenTests() {
        StrWeight w1 = new StrWeight("abcdef");
        assertEquals(WeightComparison.EQUAL, w1.compare(new StrWeight("abcdef")));

        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("abcdeg")));
        assertEquals(WeightComparison.LESS_THAN, w1.compare(new StrWeight("abcdeh")));
        assertEquals(WeightComparison.LESS_THAN, new StrWeight("babcd").compare(new StrWeight("eeeee")));

        assertEquals(WeightComparison.GREATER_THAN, new StrWeight("babcd").compare(new StrWeight("e")));

        assertEquals(WeightComparison.GREATER_THAN, new StrWeight("babcd").compare(new StrWeight("eba")));
        assertEquals(WeightComparison.GREATER_THAN, new StrWeight("babcd").compare(new StrWeight("wwww")));

    }
}
