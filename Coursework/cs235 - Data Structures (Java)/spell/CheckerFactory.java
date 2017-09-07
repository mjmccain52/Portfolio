
package cs235.spell;

public class CheckerFactory {

    /**
     * Creates and returns an object that implements the SpellingChecker interface
     *
     * @return A new object that implements the SpellingChecker interface
     */
    public static SpellingChecker createSpellingChecker() {
        return new SpellingCheckerImpl();
    }
}

