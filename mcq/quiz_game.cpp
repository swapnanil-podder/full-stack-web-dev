#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <set>
#include <random>

using namespace std;

enum Difficulty { EASY, MEDIUM, HARD };

struct Question {
    int id;
    string text;
    vector<string> options;
    int correctIndex;
    Difficulty difficulty;
    string topic;
    string hint;
    string explanation;
    string comparisonTable; // For distractors
    bool isHybrid;
    string errorType; // "Syntax" or "Logic"
};

struct UserState {
    int currentStreak = 0;
    map<string, int> topicPerformance; // Topic -> Score (positive for good, negative for bad)
    vector<int> missedQuestionIds;
    vector<int> reviewQueue;
    int totalQuestionsAnswered = 0;
    double totalScore = 0;
    bool isBlitzMode = false;
    map<int, double> timeTakenPerQuestion; // ID -> Seconds
};

// Global Question Bank
vector<Question> questionBank;

// Function to populate questions
void initializeQuestions() {
    questionBank = {
        // --- Chapter 3: Basics ---
        {1, "Which tag is the correct way to define the most important heading?", 
         {"<head>", "<h1>", "<title>", "<header>"}, 1, EASY, "Headings", 
         "Think of the hierarchy. 1 is usually the top.", 
         "<h1> defines the most important heading. <head> is for metadata.", 
         "<h1>: Content Heading | <head>: Page Metadata container", false, "Syntax"},

        {2, "What happens if you use an <h7> tag?", 
         {"It displays smaller than <h6>", "It causes an error", "Browsers display it as normal text (invalid)", "It is a valid new HTML5 tag"}, 2, MEDIUM, "Headings", 
         "HTML only defines up to h6.", 
         "Browsers do not recognize <h7>, so they treat it as a generic inline or block element without default heading styling.", 
         "", false, "Logic"},

        {3, "Which tag preserves whitespace and line breaks exactly as written?", 
         {"<p>", "<br>", "<pre>", "<code>"}, 2, EASY, "Text Formatting", 
         "Short for 'preformatted'.", 
         "<pre> preserves both spaces and line breaks. <p> collapses whitespace.", 
         "<pre>: Preserves whitespace | <p>: Collapses whitespace", false, "Syntax"},

        {4, "What is the semantic difference between <b> and <strong>?", 
         {"No difference", "<b> is for bold, <strong> is for importance", "<strong> is for bold, <b> is for importance", "<b> is deprecated"}, 1, MEDIUM, "Text Formatting", 
         "One is purely visual, the other implies meaning/urgency.", 
         "<b> is stylistically bold. <strong> indicates that the text is of strong importance.", 
         "<b>: Visual only | <strong>: Semantic importance", false, "Logic"},

        {5, "Which attribute is used to provide a tooltip when hovering over an element?", 
         {"alt", "tooltip", "title", "src"}, 2, EASY, "Attributes", 
         "It's the 'title' of the element.", 
         "The 'title' attribute displays as a tooltip in most browsers.", 
         "title: Tooltip text | alt: Alternative text for images", false, "Syntax"},

        {6, "How do you create a line break without starting a new paragraph?", 
         {"<lb>", "<break>", "<br>", "\\n"}, 2, EASY, "Text Formatting", 
         "2 letters. Break.", 
         "<br> inserts a single line break.", 
         "", false, "Syntax"},

        {7, "Which tag represents a thematic break (horizontal rule)?", 
         {"<br>", "<line>", "<hr>", "<break>"}, 2, EASY, "Structure", 
         "Horizontal Rule.", 
         "<hr> creates a horizontal line indicating a thematic break.", 
         "<hr>: Thematic break (line) | <br>: Line break (whitespace)", false, "Syntax"},

        {8, "What is the correct syntax for a comment in HTML?", 
         {"// Comment", "/* Comment */", "<!-- Comment -->", "<# Comment >"}, 2, EASY, "Comments", 
         "Starts with bang-dash-dash.", 
         "HTML comments use <!-- --> syntax.", 
         "", false, "Syntax"},

        {9, "Which tag is used to mark text as deleted (strikethrough)?", 
         {"<strike>", "<del>", "<s>", "<remove>"}, 1, MEDIUM, "Text Formatting", 
         "<strike> is deprecated. Use the semantic tag for 'deleted'.", 
         "<del> represents text that has been deleted from a document. <strike> is obsolete.", 
         "<del>: Semantic deletion | <s>: Visual strikethrough (less semantic)", false, "Syntax"},

        {10, "Which element represents a short inline quotation?", 
         {"<blockquote>", "<cite>", "<q>", "<quote>"}, 2, MEDIUM, "Quotations", 
         "Blockquote is for blocks. This is for inline.", 
         "<q> is for short inline quotes. <blockquote> is for long blocks.", 
         "<q>: Inline quote | <blockquote>: Block level quote", false, "Logic"},

        {11, "Which tag is used for the abbreviation 'HTML'?", 
         {"<acronym>", "<abbr>", "<short>", "<title>"}, 1, EASY, "Quotations", 
         "Abbreviation.", 
         "<abbr> is used for abbreviations. <acronym> is deprecated in HTML5.", 
         "", false, "Syntax"},

        {12, "How do you make text go backwards (right-to-left override)?", 
         {"<bdo dir='rtl'>", "<rtl>", "<reverse>", "<span dir='back'>"}, 0, HARD, "Quotations", 
         "Bi-Directional Override.", 
         "<bdo> stands for Bi-Directional Override.", 
         "", false, "Syntax"},

        {13, "What does the 'target=\"_blank\"' attribute do in an anchor tag?", 
         {"Opens link in the same tab", "Opens link in a new tab/window", "Downloads the file", "Sends an email"}, 1, EASY, "Links", 
         "Blank slate.", 
         "_blank opens the document in a new window or tab.", 
         "", false, "Logic"},

        {14, "Which protocol is used in a link to make a phone call?", 
         {"phone:", "call:", "tel:", "mobile:"}, 2, MEDIUM, "Links", 
         "Telephone.", 
         "The 'tel:' protocol triggers the device's dialer.", 
         "tel: Phone numbers | mailto: Email addresses", false, "Syntax"},

        {15, "What is the correct HTML for an image?", 
         {"<img href='image.jpg'>", "<image src='image.jpg'>", "<img src='image.jpg'>", "<img>image.jpg</img>"}, 2, EASY, "Images", 
         "Img tag, Source attribute.", 
         "<img> is a void tag (no closing) and uses 'src'.", 
         "<img src='...'>: Correct | <img href='...'>: Wrong attribute", false, "Syntax"},

        // --- Chapter 4: Structure ---
        {16, "Which of the following is a Block-level element?", 
         {"<span>", "<a>", "<div>", "<img>"}, 2, EASY, "Block vs Inline", 
         "It's a generic container that breaks lines.", 
         "<div> is a block element. The others are inline (by default).", 
         "<div>: Block | <span>: Inline", false, "Logic"},

        {17, "Which element is Inline by default?", 
         {"<p>", "<h1>", "<span>", "<ul>"}, 2, EASY, "Block vs Inline", 
         "Generic inline container.", 
         "<span> is inline. Paragraphs, headings, and lists are block.", 
         "", false, "Logic"},

        {18, "What is the primary purpose of a <div>?", 
         {"To make text bold", "Generic block container for grouping", "To create a link", "To list items"}, 1, EASY, "Containers", 
         "Division.", 
         "<div> is a generic container used to group elements for styling or layout.", 
         "", false, "Logic"},

        {19, "Which entity represents the copyright symbol?", 
         {"&copy;", "&copyright;", "&c;", "(c)"}, 0, MEDIUM, "Entities", 
         "Copy that.", 
         "&copy; renders the © symbol.", 
         "", false, "Syntax"},

        {20, "Which entity represents the 'greater than' (>) symbol?", 
         {"&gt;", "&lt;", "&great;", "&gtr;"}, 0, EASY, "Entities", 
         "Greater Than.", 
         "&gt; stands for Greater Than.", 
         "&gt; : > | &lt; : <", false, "Syntax"},

        {21, "Which list type uses bullet points by default?", 
         {"<ol>", "<ul>", "<dl>", "<list>"}, 1, EASY, "Lists", 
         "Unordered.", 
         "<ul> (Unordered List) uses bullets. <ol> uses numbers.", 
         "<ul>: Bullets | <ol>: Numbers", false, "Syntax"},

        {22, "In a Description List (<dl>), what defines the term?", 
         {"<dd>", "<dt>", "<li>", "<term>"}, 1, MEDIUM, "Lists", 
         "Definition Term.", 
         "<dt> is the Definition Term. <dd> is the Description/Definition.", 
         "<dt>: Term | <dd>: Description", false, "Syntax"},

        {23, "Which semantic tag represents independent, self-contained content?", 
         {"<section>", "<div>", "<article>", "<aside>"}, 2, HARD, "Semantics", 
         "Like a blog post or news story.", 
         "<article> specifies independent, self-contained content. <section> is for thematic grouping.", 
         "<article>: Independent | <section>: Thematic group", false, "Logic"},

        {24, "What is the correct semantic tag for a sidebar or content indirectly related to the main content?", 
         {"<sidebar>", "<nav>", "<aside>", "<section>"}, 2, MEDIUM, "Semantics", 
         "Set aside.", 
         "<aside> is used for content aside from the content it is placed in (like sidebars).", 
         "", false, "Logic"},

        {25, "Which tag represents the dominant content of the <body>?", 
         {"<content>", "<main>", "<article>", "<center>"}, 1, EASY, "Semantics", 
         "Main content.", 
         "<main> specifies the main content of the document.", 
         "", false, "Syntax"},

        // --- Hybrid / Advanced / Superfile Specifics ---
        {26, "How do you combine a Bold and Italic effect semantically?", 
         {"<bi>Text</bi>", "<b><i>Text</i></b>", "<strong><em>Text</em></strong>", "<style>Text</style>"}, 2, HARD, "Hybrid", 
         "Semantic importance + semantic emphasis.", 
         "Using <strong> and <em> nests semantic meaning (Important AND Emphasized).", 
         "<strong><em>: Semantic | <b><i>: Visual only", true, "Logic"},

        {27, "Which tag is used to caption a <figure> element?", 
         {"<caption>", "<figcap>", "<figcaption>", "<title>"}, 2, MEDIUM, "Semantics", 
         "Figure Caption.", 
         "<figcaption> goes inside a <figure>.", 
         "<caption>: Table title | <figcaption>: Figure title", false, "Syntax"},

        {28, "What does <mark> do?", 
         {"Creates a bookmark", "Highlights text", "Marks text for deletion", "Creates a checkbox"}, 1, MEDIUM, "Text Formatting", 
         "Highlighter pen.", 
         "<mark> renders text with a highlighted background (usually yellow).", 
         "", false, "Syntax"},

        {29, "The <address> tag is typically used for?", 
         {"Any postal address", "IP addresses", "Contact information for the author/owner", "URL locations"}, 2, HARD, "Semantics", 
         "Not just any address.", 
         "<address> provides contact info for the nearest <article> or <body> ancestor.", 
         "", false, "Logic"},

        {30, "Is the <nav> element required for ALL links?", 
         {"Yes, always", "No, only for major navigation blocks", "No, <nav> is deprecated", "Yes, for accessibility"}, 1, MEDIUM, "Semantics", 
         "Think footer links vs main menu.", 
         "<nav> is for major blocks of navigation links, not every single link.", 
         "", false, "Logic"},

        {31, "What happens if you nest a Block element inside an Inline element (e.g., <div> inside <span>)?", 
         {"It is strictly invalid HTML4, but HTML5 allows it in some cases (like <a> wrapping <div>)", "The browser crashes", "The text turns red", "It automatically becomes inline"}, 0, HARD, "Block vs Inline", 
         "HTML5 relaxed some rules, especially for <a>.", 
         "Traditionally invalid, but HTML5 allows block elements inside <a>. Generally, avoid putting <div> inside <span>.", 
         "", true, "Logic"},

        {32, "Which entity is &nbsp;?", 
         {"No Block Space", "Non-Breaking Space", "New Break Space", "Null Byte Space"}, 1, MEDIUM, "Entities", 
         "Prevents line break at that space.", 
         "&nbsp; is a Non-Breaking Space.", 
         "", false, "Syntax"},

        {33, "Which tag is a 'void' element (has no closing tag)?", 
         {"<p>", "<div>", "<img>", "<span>"}, 2, EASY, "Syntax", 
         "Images don't hold text inside them.", 
         "<img> is a void element. Others require closing tags.", 
         "", false, "Syntax"},

        {34, "What is the purpose of the 'alt' attribute in <img>?", 
         {"Tooltip on hover", "Alternative text if image fails to load + Screen Readers", "Title of the image", "Caption below image"}, 1, MEDIUM, "Images", 
         "Accessibility and Broken links.", 
         "'alt' provides text if the image is missing and is read by screen readers.", 
         "alt: Replacement text | title: Tooltip", false, "Logic"},

        {35, "Which tag specifies a standard paragraph?", 
         {"<paragraph>", "<text>", "<p>", "<para>"}, 2, EASY, "Basics", 
         "Single letter.", 
         "<p> defines a paragraph.", 
         "", false, "Syntax"},

        {36, "Which formatting tag creates subscript text (like H2O)?", 
         {"<sup>", "<down>", "<sub>", "<small>"}, 2, MEDIUM, "Text Formatting", 
         "Submarine goes down.", 
         "<sub> defines subscript text.", 
         "<sub>: Lower (H2O) | <sup>: Higher (X2)", false, "Syntax"},

        {37, "Which formatting tag creates superscript text (like X^2)?", 
         {"<sup>", "<up>", "<sub>", "<top>"}, 0, MEDIUM, "Text Formatting", 
         "Superman goes up.", 
         "<sup> defines superscript text.", 
         "", false, "Syntax"},

        {38, "Which element defines a footer for a document or section?", 
         {"<bottom>", "<footer>", "<end>", "<section-end>"}, 1, EASY, "Semantics", 
         "Opposite of header.", 
         "<footer> defines a footer.", 
         "", false, "Syntax"},

        {39, "How do you specify a link that sends an email?", 
         {"<a href='email:x@y.com'>", "<a href='mailto:x@y.com'>", "<a href='send:x@y.com'>", "<mail>x@y.com</mail>"}, 1, MEDIUM, "Links", 
         "Mail To.", 
         "href='mailto:...' is the correct protocol.", 
         "", false, "Syntax"},

        {40, "What is the correct way to make an image clickable?", 
         {"<img href='url'>", "<a src='url'><img></a>", "<a href='url'><img src='img.jpg'></a>", "<click><img></click>"}, 2, MEDIUM, "Hybrid", 
         "Wrap the image.", 
         "Wrap the <img> tag inside an <a> tag.", 
         "", true, "Syntax"},

        {41, "What does <small> do in HTML5?", 
         {"Just makes text smaller", "Represents side comments / fine print", "Deprecated", "Same as <sub>"}, 1, HARD, "Text Formatting", 
         "It has semantic meaning now.", 
         "In HTML5, <small> represents side comments, such as copyright and legal text (fine print), not just visual sizing.", 
         "", false, "Logic"},

        {42, "Which tag is used to embed another HTML page within the current page?", 
         {"<frame>", "<iframe>", "<embed>", "<object>"}, 1, MEDIUM, "Structure", 
         "Inline Frame.", 
         "<iframe> is used to embed another document.", 
         "", false, "Syntax"},

        {43, "Which attribute allows a link to download the resource instead of navigating?", 
         {"save", "down", "download", "type='save'"}, 2, MEDIUM, "Links", 
         "Attribute name is the action.", 
         "The 'download' attribute prompts the user to save the linked URL.", 
         "", false, "Syntax"},

        {44, "What is the function of <time>?", 
         {"Displays a clock", "Semantically represents a specific time/date", "Delays page loading", "Shows server time"}, 1, HARD, "Semantics", 
         "Machine readable time.", 
         "<time> allows you to represent a machine-readable date/time.", 
         "", false, "Logic"},

        {45, "Which tag indicates a definition of a term?", 
         {"<def>", "<dfn>", "<define>", "<mean>"}, 1, HARD, "Text Formatting", 
         "DeFiNition.", 
         "<dfn> represents the defining instance of a term.", 
         "", false, "Syntax"},

        {46, "How do you comment out a large block of code in HTML?", 
         {"You can't", "Using /* */", "Using <!-- --> around the block", "Using #"}, 2, EASY, "Comments", 
         "Standard HTML comment tags.", 
         "<!-- ... --> can span multiple lines.", 
         "", false, "Syntax"},

        {47, "What is the default display value of <a>?", 
         {"block", "inline", "inline-block", "none"}, 1, MEDIUM, "Block vs Inline", 
         "It sits inside text.", 
         "Anchors <a> are inline by default.", 
         "", false, "Logic"},

        {48, "Which element represents keyboard input?", 
         {"<input>", "<type>", "<kbd>", "<code >"}, 2, MEDIUM, "Text Formatting", 
         "Keyboard.", 
         "<kbd> represents user input (keyboard keys).", 
         "<kbd>: User Input | <code>: Source Code", false, "Syntax"},

        {49, "Which element is used to highlight parts of a text for reference or notation purposes?", 
         {"<b>", "<strong>", "<mark>", "<em>"}, 2, MEDIUM, "Text Formatting", 
         "Yellow highlighter.", 
         "<mark> is for highlighting.", 
         "", false, "Logic"},

        {50, "What is the correct HTML structure sequence?", 
         {"<html><head><body>", "<html><body><head>", "<head><body><html>", "<body><head><html>"}, 0, EASY, "Basics", 
         "Root -> Metadata -> Content.", 
         "<html> contains <head> and <body>. <head> comes before <body>.", 
         "", false, "Syntax"}
    };
}

// --- Game Logic ---

void printProgressBar(int current, int total) {
    float percentage = (float)current / total;
    int barWidth = 20;
    
    cout << "[";
    int pos = barWidth * percentage;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "#";
        else cout << "-";
    }
    cout << "] " << int(percentage * 100.0) << " %\r";
    cout.flush();
}

void saveProgress(const UserState& state) {
    ofstream file("mcq/progress.dat");
    if (file.is_open()) {
        file << state.currentStreak << "\n";
        file << state.totalQuestionsAnswered << "\n";
        file << state.totalScore << "\n";
        // Simple save for demo. In production, serialize maps/vectors properly.
        file.close();
        cout << "\n[Game Saved]\n";
    }
}

void showComparison(const string& comparison) {
    if (comparison.empty()) return;
    cout << "\n--- Distractor Comparison ---\n";
    cout << comparison << "\n";
    cout << "-----------------------------\n";
}

int main() {
    initializeQuestions();
    UserState userState;

    cout << "Welcome to the HTML Masterclass Quiz!\n";
    cout << "Select Mode: 1. Normal  2. Blitz Mode (Score based on time)\n";
    int modeChoice;
    cin >> modeChoice;
    if (modeChoice == 2) userState.isBlitzMode = true;

    vector<int> questionQueue;
    for (const auto& q : questionBank) questionQueue.push_back(q.id);

    // Random shuffle for variety, but we will sort/filter for difficulty later
    random_device rd;
    mt19937 g(rd());
    shuffle(questionQueue.begin(), questionQueue.end(), g);

    // Filter queue logic (Topic Weighting & Progressive Difficulty)
    // For this implementation, we simply iterate, but in a real loop we would pick 
    // the next question dynamically.
    
    int qIndex = 0;
    while (qIndex < questionQueue.size() || !userState.reviewQueue.empty()) {
        int qID;
        bool isReview = false;
        
        if (!userState.reviewQueue.empty()) {
            qID = userState.reviewQueue.front();
            userState.reviewQueue.erase(userState.reviewQueue.begin());
            isReview = true;
            cout << "\n[REVIEW ROUND] Let's try this one again...\n";
        } else {
            if (qIndex >= questionQueue.size()) break;
            qID = questionQueue[qIndex];
            qIndex++;
        }

        // Find question
        auto it = find_if(questionBank.begin(), questionBank.end(), [qID](const Question& q){ return q.id == qID; });
        if (it == questionBank.end()) continue;
        Question currentQ = *it;

        // Progressive Difficulty Gating
        if (currentQ.difficulty == HARD && userState.currentStreak < 3 && !isReview) {
            // Push hard question to back if streak is low
            questionQueue.push_back(qID); 
            continue; 
        }

        // Display
        cout << "\n--------------------------------------------------\n";
        printProgressBar(userState.totalQuestionsAnswered, 50);
        cout << "\nQ" << userState.totalQuestionsAnswered + 1 << ": " << currentQ.text;
        if (currentQ.isHybrid) cout << " [HYBRID]";
        cout << "\n";
        
        for (int i = 0; i < currentQ.options.size(); ++i) {
            cout << i + 1 << ". " << currentQ.options[i] << "\n";
        }

        auto start = chrono::high_resolution_clock::now();
        
        cout << "Your Answer (1-" << currentQ.options.size() << ") or 0 for Hint: ";
        int answer;
        cin >> answer;

        if (answer == 0) {
            cout << "HINT: " << currentQ.hint << "\n";
            cout << "Your Answer: ";
            cin >> answer;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        userState.timeTakenPerQuestion[qID] = elapsed.count();

        cout << "Confidence (1: Low, 2: High): ";
        int confidence;
        cin >> confidence;

        bool isCorrect = (answer - 1 == currentQ.correctIndex);
        
        // Confidence Logic
        if (isCorrect && confidence == 1) {
            cout << "\n[!] Correct, but you seemed unsure.\n";
            cout << "Explanation: " << currentQ.explanation << "\n";
        } else if (isCorrect) {
            cout << "\n[+] Correct!\n";
            userState.currentStreak++;
            userState.topicPerformance[currentQ.topic]++;
            
            // Scoring
            double points = 10;
            if (userState.isBlitzMode) {
                points = (currentQ.difficulty + 1) * 100 / max(1.0, elapsed.count());
            }
            userState.totalScore += points;

        } else {
            cout << "\n[-] Incorrect.\n";
            cout << "Correct Answer: " << currentQ.options[currentQ.correctIndex] << "\n";
            cout << "Explanation: " << currentQ.explanation << "\n";
            cout << "Error Type: " << currentQ.errorType << "\n";
            
            // Distractor Comparison
            if (!currentQ.comparisonTable.empty()) {
                showComparison(currentQ.comparisonTable);
            }

            userState.currentStreak = 0;
            userState.topicPerformance[currentQ.topic]--;
            userState.missedQuestionIds.push_back(qID);
            
            // Spaced Repetition Trigger
            userState.reviewQueue.push_back(qID);
        }

        userState.totalQuestionsAnswered++;
    }

    // Results
    cout << "\n============================================\n";
    cout << "QUIZ COMPLETE\n";
    cout << "Final Score: " << userState.totalScore << "\n";
    
    cout << "\nTopic Mastery:\n";
    for (auto const& [topic, score] : userState.topicPerformance) {
        cout << topic << ": " << (score > 0 ? "Mastered" : "Needs Review") << " (" << score << ")\n";
    }

    if (!userState.missedQuestionIds.empty()) {
        cout << "\nYou struggled with these questions (IDs): ";
        for (int id : userState.missedQuestionIds) cout << id << " ";
        cout << "\n";
    }

    saveProgress(userState);
    return 0;
}
