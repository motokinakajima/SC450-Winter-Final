# AI Usage Log

This file documents the interaction with AI (Gemini/Google) for the CS101 Research Project. 
The project evolved from an initial exploration of Genetic Algorithms into a graph-theoretical study of the Harkness Table seating arrangement.

---

## Statement on AI Assistance (Overview)

As the AI model involved in this brainstorming process, I provide the following statement regarding my role:

I served as a technical consultant and sounding board. My primary contributions were:
1. **Feasibility Analysis:** Evaluating the mathematical rigor of different project ideas.
2. **Concept Formalization:** Mapping the student’s original geometric ideas (e.g., Harkness table dynamics) onto formal Graph Theory terminology.
3. **Algorithmic Categorization:** Helping the student identify that their "angle bisection" idea falls under the "Greedy" paradigm and providing technical details on "Beam Search" as a comparative model.

The student maintained full control over the creative and mathematical direction, including the original idea of using $\cos \theta$ for "facing-ness" and Taylor expansion for constant approximation.

---

## Log Entries

### Date: February 2026 (Initial Brainstorming)
**Language of Interaction:** Japanese

**Prompt (Translated excerpt):**
"I'm thinking of doing a research project on Genetic Algorithms (GA) for my CS101 class. Is it a good idea? I also have this idea about a 'Harkness Table' seating arrangement at my school. Could that work?"

**AI Response (Excerpt):**
"While Genetic Algorithms (GA) are interesting, proving tight asymptotic bounds for them within a one-week project is extremely difficult and might not meet the rigor requirements of CS101. On the other hand, the Harkness Table idea is highly original. You can model it using Graph Theory, which is a core topic in the course."

**Verification & Modification:**
- I realized that my previous GA project lacked the mathematical rigor required for a "tight bound proof" in this specific course. 
- I decided to pivot to the Harkness Table model. 
- I independently came up with the idea to use a normalized "Speaking Score" (0 to 1) for each student to simplify the input model.

---

### Date: February 2026 (Mathematical Modeling)
**Language of Interaction:** Japanese

**Prompt (Translated excerpt):**
"I want to reflect the 'facing-ness' of the circular table in the graph weights. I'm thinking of using $\cos \theta$ of the angle between two people as the weight. Also, if I use Taylor expansion for $\cos$, I can approximate it as a constant for people facing each other, right?"

**AI Response (Excerpt):**
"That is an excellent mathematical model. Using $\cos \theta$ for a circular arrangement perfectly translates geometry into graph edge weights. Applying Taylor expansion (Maclaurin series) to treat $\cos(x) \approx 1$ for small $x$ is a clever way to simplify the theoretical proof (Tight bound analysis) while keeping the model realistic."

**Verification & Modification:**
- I verified the Maclaurin series for $\cos(x)$ in my calculus materials.
- I decided that the objective function would be $S = \sum (v_i \cdot v_j \cdot \cos \theta_{ij})$. 
- I confirmed that $O(n^2)$ is the optimal complexity for evaluating a complete graph with $n$ vertices.

---

### Date: February 2026 (Algorithmic Comparison)
**Language of Interaction:** Japanese

**Prompt (Translated excerpt):**
"For the algorithm, I thought of a 'greedy' way where I place the strongest pair first and then bisect the angles. But can we make it more like A* search? How does Beam Search work?"

**AI Response (Excerpt):**
"Your angle-bisection idea is a strong heuristic. To make it more like a search algorithm, Beam Search is a great choice. It keeps the top $B$ best states at each step, avoiding the 'local optima' trap of a standard Greedy algorithm while maintaining a manageable time complexity of $O(B \cdot n^2)$."

**Verification & Modification:**
- I researched Beam Search independently to understand how it limits the search space compared to Breadth-First Search.
- I decided to compare "Standard Greedy (Maximum Marginal Gain)" vs. "Beam Search (B=3)" in my Experimental Plan.
- I drafted the Research Question to focus on the trade-off between the score and runtime.

---

## Academic Integrity Affirmation
All LaTeX writing, formal proofs, and final implementation plans were produced independently by the student. AI was used strictly for clarifying definitions and exploring the feasibility of the student's original concepts.