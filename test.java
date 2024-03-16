import java.util.*;

class test {
    public static void addEven(ArrayList<Integer> array, Integer element) {
        for (int index = 0; index < array.size(); index++) {
            if (index % 2 == 0) {
                array.add(index, element);
                index++;
            }
        }
    }

    public static void correctlyAddEven(ArrayList<Integer> array, Integer element) {
        for (int index = 0; index < array.size(); index++) {
            if (index % 2 == 0) {
                array.add(index, element);
                index++;
                index++;
            }
        }
    }

    public static void main(String[] args) {
        ArrayList<Integer> array = new ArrayList<Integer>();
        for (int index = 0; index < 10; index++) {
            array.add(index);
        }
        addEven(array, 99);
        System.out.println("choice C:" + array);
        array.clear();
        for (int index = 0; index < 10; index++) {
            array.add(index);
        }
        correctlyAddEven(array, 99);
        System.out.println("choice A:" + array);
    }
}