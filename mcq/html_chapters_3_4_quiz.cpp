#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

enum Difficulty { EASY, MEDIUM, HARD };

struct Question {
    string questionText;
    vector<string> options;
    int correctOption;
    string hint;
    string explanation;
    Difficulty difficulty;
    string similarityNote; // Explains difference between similar options
};

void printDivider() {
    cout << "\n" << string(60, '=') << "\n";
}

void showSuperfileBriefing() {
    cout << "--- SUPERFILE BRIEFING: UNIQUE CONCEPTS ---\n";
    cout << "Before we begin, here are some unique concepts found in your 'Superfiles':\n\n";
    cout << "1.  <h7>: While only h1-h6 are valid, browsers will often render <h7> as plain text.\n";
    cout << "2.  <bdo dir=\"rtl\">: Bi-Directional Override. Used to reverse text direction (e.g., for RTL languages).\n";
    cout << "3.  <pre>: Preserves both spaces and line breaks exactly as written in the HTML file.\n";
    cout << "4.  <abbr title=\"...\">: Defines an abbreviation; the 'title' attribute shows the full version on hover.\n";
    cout << "5.  <address>: Used specifically for contact information of the author/owner.\n";
    cout << "6.  <cite>: Used to define the title of a creative work (e.g., a book or movie).\n";
    cout << "7.  <kbd>, <samp>, <var>: Semantic tags for keyboard input, sample output, and variables respectively.\n";
    cout << "8.  Image Maps: Uses <map> and <area> with the <img> attribute 'usemap' to create clickable regions.\n";
    cout << "9.  <fieldset> & <legend>: Group related form elements with a caption (<legend>).\n";
    cout << "10. <tfoot>: In a table, the footer is defined using <tfoot> and often contains summary data.\n";
    cout << "\nPress Enter to start the quiz...";
    cin.get();
}

string difficultyToString(Difficulty d) {
    switch (d) {
        case EASY: return "Easy";
        case MEDIUM: return "Medium";
        case HARD: return "Hard";
        default: return "Unknown";
    }
}

int main() {
    showSuperfileBriefing();

    vector<Question> questions = {
        // CHAPTER 3 & SUPERFILE 1-10
        {"Which tag is used for the smallest standard heading?", {"<h1>", "<h6>", "<h7>", "<head>"}, 1, "Think 1 to 6.", "<h6> is the smallest standard heading. <h7> is invalid but often rendered as plain text.", EASY, "<h1> is largest, <h6> is smallest."},
        {"What does the 'bdo' tag stand for?", {"Binary Data Object", "Bi-Directional Override", "Block Display Object", "Basic Document Order"}, 1, "It changes text direction.", "BDO stands for Bi-Directional Override, used to flip text direction.", HARD, "Don't confuse with 'b' (bold) or 'blockquote'."},
        {"Which attribute in the <a> tag is used to open a link in a new tab?", {"href", "newtab", "target=\"_blank\"", "rel=\"external\""}, 2, "It 'targets' a specific window.", "target=\"_blank\" tells the browser to open the link in a new window/tab.", EASY, "href is the URL; target is the destination behavior."},
        {"How do you preserve whitespace and line breaks in HTML?", {"<br>", "<p>", "<pre>", "<div>"}, 2, "Think 'pre-formatted'.", "<pre> preserves all white space and line breaks.", MEDIUM, "<div> is a block container but collapses whitespace; <pre> keeps it."},
        {"Which tag is semantically correct for marking 'Strong Importance'?", {"<b>", "<strong>", "<i>", "<em>"}, 1, "It conveys meaning, not just style.", "<strong> indicates importance, while <b> is just for visual boldness.", EASY, "<b> is style; <strong> is semantic importance."},
        {"Which tag is used to define an abbreviation?", {"<abb>", "<short>", "<abbr>", "<acronym>"}, 2, "It's a 4-letter tag.", "<abbr> is the standard; <acronym> is deprecated in HTML5.", MEDIUM, "<abbr> uses the 'title' attribute for the full text."},
        {"What is the correct way to link to an email address?", {"<a href=\"email:test@ex.com\">", "<a mailto=\"test@ex.com\">", "<a href=\"mailto:test@ex.com\">", "<a>test@ex.com</a>"}, 2, "Starts with 'mailto:'.", "The href value must start with 'mailto:' followed by the address.", MEDIUM, "Don't use 'email:' - that's not a standard URI scheme."},
        {"Which tag defines a thematic break (horizontal line)?", {"<line>", "<br>", "<hr>", "<border>"}, 2, "Horizontal Rule.", "<hr> stands for Horizontal Rule.", EASY, "<br> is a line break; <hr> is a thematic separator."},
        {"What is the purpose of the 'alt' attribute in <img>?", {"Title on hover", "Alternative text if image fails", "Image alignment", "Link destination"}, 1, "It helps screen readers.", "Alt text provides a description for accessibility or if the image fails to load.", EASY, "title attribute provides the tooltip; alt provides the context."},
        {"Which tag defines the title of a work like a book or song?", {"<work>", "<cite>", "<ref>", "<i>"}, 1, "Think 'citation'.", "<cite> is used for the title of creative works.", MEDIUM, "<cite> often renders in italics, but has specific semantic meaning."},

        // CHAPTER 4 & SUPERFILE 11-20
        {"Which of these is a generic INLINE container?", {"<div>", "<span>", "<section>", "<article>"}, 1, "Starts with 's'.", "<span> is the generic inline container. <div> is block-level.", EASY, "div = Block; span = Inline."},
        {"Which entity represents the 'Less Than' symbol (<)?", {"&gt;", "&lt;", "&amp;", "&copy;"}, 1, "L-T.", "&lt; stands for Less Than.", EASY, "&lt; (less than) vs &gt; (greater than)."},
        {"Which tag is used for an unordered list?", {"<ol>", "<ul>", "<li>", "<dl>"}, 1, "Think 'bullets'.", "<ul> stands for Unordered List.", EASY, "ul (bullets) vs ol (numbers)."},
        {"In a Description List (<dl>), which tag defines the term?", {"<dt>", "<dd>", "<li>", "<term>"}, 0, "Description Term.", "<dt> defines the term, and <dd> defines the description.", MEDIUM, "dt = term; dd = description/data."},
        {"Which element represents the main content of the <body>?", {"<content>", "<section>", "<main>", "<article>"}, 2, "Only one per page.", "<main> should contain the primary content of the document.", MEDIUM, "section and article are for grouping; main is the primary container."},
        {"Which tag is used to group related elements in a form?", {"<group>", "<fieldset>", "<container>", "<section>"}, 1, "Found in Chapter 4 superfile.", "<fieldset> groups related form controls and labels.", HARD, "<fieldset> often has a <legend> for its title."},
        {"What does <kbd> represent?", {"Keyboard Shortcut", "Keyboard Input", "Key Block Data", "Knowledge Base"}, 1, "Think user input.", "<kbd> represents user input from a keyboard.", HARD, "samp is output; kbd is input; var is variable."},
        {"Which tag is used to create a caption for a <figure>?", {"<cap>", "<caption>", "<figcaption>", "<title>"}, 2, "It starts with fig.", "<figcaption> provides a caption for the <figure> element.", MEDIUM, "<caption> is for tables; <figcaption> is for figures."},
        {"Which semantic tag is best for a sidebar?", {"<sidebar>", "<nav>", "<aside>", "<section>"}, 2, "Beside the content.", "<aside> is for content indirectly related to the main content (like sidebars).", MEDIUM, "aside vs nav: nav is specifically for links."},
        {"How do you display a Copyright symbol in HTML?", {"&copy;", "&copyright;", "(c)", "&cp;"}, 0, "Short and common.", "&copy; is the entity for the copyright symbol.", EASY, "Always use entities for special symbols to ensure rendering."},

        // 21-30 (Mixed)
        {"Which tag defines a short inline quotation?", {"<blockquote>", "<q>", "<quote>", "<cite>"}, 1, "Single letter.", "<q> is for short quotes and adds quotation marks.", EASY, "blockquote = block-level; q = inline."},
        {"What is the 'title' attribute used for on a <p> tag?", {"Page title", "Heading", "Tooltip on hover", "Search engine keyword"}, 2, "Extra info.", "The 'title' attribute provides extra info, usually as a tooltip.", MEDIUM, "Don't confuse with the <title> tag in the <head>."},
        {"Which tag defines a table footer?", {"<footer-table>", "<tfoot>", "<tfoot>", "<td>"}, 2, "Chapter 4 superfile.", "<tfoot> groups the footer content in a table.", MEDIUM, "thead, tbody, tfoot."},
        {"What is the purpose of <mark>?", {"Bold text", "Important text", "Highlighted text", "Underlined text"}, 2, "Think yellow pen.", "<mark> is used to highlight or mark text.", MEDIUM, "strong is importance; mark is visual highlighting."},
        {"Which tag is used to define a mathematical variable?", {"<math>", "<v>", "<var>", "<code>"}, 2, "Three letters.", "<var> is used for variables in math or programming.", HARD, "code is for snippets; var is specifically for variables."},
        {"Which tag is used for a subscript (like in H2O)?", {"<sup>", "<sub>", "<small>", "<down>"}, 1, "Sub = Below.", "<sub> is subscript; <sup> is superscript.", EASY, "sub (below) vs sup (above)."},
        {"What does <address> define?", {"Home address", "Server location", "Contact info for author", "Link to Google Maps"}, 2, "Chapter 3 superfile.", "<address> provides contact info for the owner/author of a doc.", MEDIUM, "It is semantic, not just for physical addresses."},
        {"Which list starts numbering from 1 by default?", {"<ul>", "<dl>", "<ol>", "<li>"}, 2, "Ordered.", "<ol> is an ordered (numbered) list.", EASY, "ul (unordered) vs ol (ordered)."},
        {"Which tag defines an independent, self-contained piece of content?", {"<section>", "<div>", "<article>", "<aside>"}, 2, "Like a blog post.", "<article> should make sense on its own (blog, news).", MEDIUM, "article vs section: article is more self-contained."},
        {"How do you define an image map on an image?", {"map=\"name\"", "usemap=\"#name\"", "id=\"name\"", "href=\"#name\""}, 1, "Uses the 'use' keyword.", "The <img> tag uses 'usemap' to link to a <map>.", HARD, "usemap needs the '#' prefix to refer to the map name."},

        // 31-40 (More detailed)
        {"What is the default display of <div>?", {"inline", "block", "inline-block", "none"}, 1, "Takes full width.", "<div> is a block-level element.", EASY, "div = Block; span = Inline."},
        {"Which entity is a non-breaking space?", {"&space;", "&nbsp;", "&s;", "&blank;"}, 1, "n-b-s-p.", "&nbsp; stands for non-breaking space.", MEDIUM, "Used to prevent words from splitting across lines."},
        {"Which attribute in <a> is used for telephone numbers?", {"href=\"call:\"", "href=\"phone:\"", "href=\"tel:\"", "tel=\"\""}, 2, "Three letters.", "tel: is the standard prefix for phone links.", MEDIUM, "mailto: for email, tel: for phone."},
        {"What does <ins> represent?", {"Inside", "Inserted text", "Instant", "Instruction"}, 1, "Usually underlined.", "<ins> represents text that has been inserted into a document.", MEDIUM, "del (deleted) vs ins (inserted)."},
        {"Which tag is used to group the header content of a page?", {"<head>", "<top>", "<header>", "<heading>"}, 2, "ER ending.", "<header> is the semantic tag for introductory content.", EASY, "<head> is for metadata; <header> is for visible content."},
        {"What is <samp> used for?", {"Sample input", "Sample output", "Sample code", "Simple text"}, 1, "Opposite of kbd.", "<samp> is for sample output from a computer program.", HARD, "kbd = input; samp = output."},
        {"Which tag defines a navigation link container?", {"<links>", "<nav>", "<ul>", "<menu>"}, 1, "Three letters.", "<nav> is specifically for major navigation blocks.", EASY, "ul is often used *inside* nav, but nav is the semantic container."},
        {"Which of these is NOT a block-level element?", {"<p>", "<div>", "<h1>", "<a>"}, 3, "Stays on the same line.", "<a> is an inline element.", MEDIUM, "p, div, h1 are all block-level."},
        {"What is the correct tag for a multi-line text input?", {"<input type=\"text\">", "<input type=\"textarea\">", "<textarea>", "<textblock>"}, 2, "Own tag name.", "<textarea> is its own element, not an input type.", MEDIUM, "input is for single line; textarea is for multiple lines."},
        {"Which tag defines a dropdown list?", {"<dropdown>", "<list>", "<select>", "<option>"}, 2, "Chapter 4 superfile.", "<select> creates the dropdown; <option> creates items.", MEDIUM, "select = container; option = individual item."},

        // 41-50 (Final Stretch)
        {"Which tag is used for superscript (like X²)?", {"<sub>", "<sup>", "<pow>", "<up>"}, 1, "Sup = Above.", "<sup> is for superscript.", EASY, "sub (below) vs sup (above)."},
        {"What does the 'cite' attribute in <blockquote> do?", {"Displays the author", "Points to the source URL", "Italics the text", "Defines the title"}, 1, "It's a URL.", "The 'cite' attribute provides a link to the source of the quote.", HARD, "cite attribute (URL) vs <cite> tag (Title)."},
        {"Which tag is used for a deleted part of a document?", {"<remove>", "<del>", "<strike>", "<s>"}, 1, "Three letters.", "<del> is the semantic tag for deleted text.", MEDIUM, "<s> is for things no longer correct; del is for document edits."},
        {"Which tag represents a generic section of a document?", {"<div>", "<main>", "<section>", "<article>"}, 2, "Chapter 4 semantics.", "<section> groups related content together.", EASY, "section is semantic; div is not."},
        {"What is the use of <legend>?", {"Title for <table>", "Title for <fieldset>", "Caption for <img>", "Footer for <main>"}, 1, "Used with fieldset.", "<legend> defines a caption for a <fieldset>.", MEDIUM, "caption (table) vs figcaption (figure) vs legend (fieldset)."},
        {"Which entity is used for the Euro symbol?", {"&euro;", "&e;", "&eur;", "&symbol;"}, 0, "Currency name.", "&euro; is the entity for €.", EASY, "Remember to use & for start and ; for end."},
        {"What does <dfn> stand for?", {"Data Function", "Definition", "Define Form", "Description Field"}, 1, "Defining a term.", "<dfn> marks the defining instance of a term.", HARD, "Use it when you first introduce a new word."},
        {"Which tag is used for teletype (monospaced) text?", {"<code>", "<tt>", "<pre>", "<mono>"}, 1, "Double T.", "<tt> was used for teletype text (now deprecated/rare).", HARD, "Use <code> or CSS font-family instead today."},
        {"Which element is used to represent the result of a calculation?", {"<result>", "<calc>", "<output>", "<answer>"}, 2, "Chapter 4 superfile.", "<output> represents the result of a calculation or user action.", HARD, "Newly introduced in HTML5 for forms."},
        {"What is the correct tag for a line break?", {"<lb>", "<break>", "<br>", "<hr>"}, 2, "Two letters.", "<br> creates a single line break.", EASY, "<br> has no closing tag."}
    };

    int score = 0;
    string userChoice;

    cout << "\nWelcome to the HTML Chapters 3 & 4 Mastery Quiz!\n";
    cout << "50 Questions. Format: [A, B, C, D] or 'H' for Hint.\n";

    for (int i = 0; i < questions.size(); ++i) {
        printDivider();
        cout << "Question " << (i + 1) << " [" << difficultyToString(questions[i].difficulty) << "]:\n";
        cout << questions[i].questionText << "\n";
        
        char label = 'A';
        for (const string& opt : questions[i].options) {
            cout << "  " << label++ << ". " << opt << "\n";
        }

        bool answered = false;
        while (!answered) {
            cout << "\nYour choice (or 'H' for hint): ";
            cin >> userChoice;
            transform(userChoice.begin(), userChoice.end(), userChoice.begin(), ::toupper);

            if (userChoice == "H") {
                cout << ">> HINT: " << questions[i].hint << "\n";
                continue;
            }

            if (userChoice.length() == 1 && userChoice[0] >= 'A' && userChoice[0] <= 'D') {
                int choiceIndex = userChoice[0] - 'A';
                if (choiceIndex == questions[i].correctOption) {
                    cout << "\n[CORRECT!]\n";
                    score++;
                } else {
                    cout << "\n[WRONG!]\n";
                    cout << "The correct answer was " << (char)('A' + questions[i].correctOption) << ": " << questions[i].options[questions[i].correctOption] << "\n";
                }
                
                cout << "EXPLANATION: " << questions[i].explanation << "\n";
                if (!questions[i].similarityNote.empty()) {
                    cout << "NOTE ON SIMILARITY: " << questions[i].similarityNote << "\n";
                }
                answered = true;
            } else {
                cout << "Invalid input. Please enter A, B, C, D or H.\n";
            }
        }
    }

    printDivider();
    cout << "QUIZ COMPLETE!\n";
    cout << "Your Final Score: " << score << "/" << questions.size() << " (" << (score * 100 / questions.size()) << "%)\n";
    
    if (score == 50) cout << "HTML Master! You've conquered Chapters 3 and 4.\n";
    else if (score > 40) cout << "Great job! You have a solid grasp of the material.\n";
    else if (score > 25) cout << "Not bad, but a review of the superfiles might help.\n";
    else cout << "Keep studying! HTML structure is the foundation of the web.\n";

    return 0;
}
