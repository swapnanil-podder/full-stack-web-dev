# 📝 Markdown Essentials Cheatsheet

> **Pro-Tip:** A well-formatted `README.md` acts as your project's cover page. Spend 10 minutes on documentation to significantly boost your project's professionalism.

---

## 1. Headings
Use `#` followed by a space. The number of `#` symbols determines the level.

# Heading Level 1 (Main Title)
## Heading Level 2 (Sections)
### Heading Level 3 (Sub-sections)
#### Heading Level 4

---

## 2. Text Formatting
Emphasis and structure for standard paragraphs.

* **Bold:** `**text**` or `__text__` -> **Bold Text**
* *Italic:* `*text*` or `_text_` -> *Italic Text*
* ~~Strikethrough:~~ `~~text~~` -> ~~Strikethrough~~
* **Horizontal Rule:** `---` (Used to separate sections)

---

## 3. Lists & Tasks
Organize instructions or features.

### Unordered List
- Item A
- Item B
  - Sub-item (Indent with 2 spaces)

### Ordered List
1. First Step
2. Second Step

### Task List (GitHub Flavored)
- [x] Completed task
- [ ] Pending task

---

## 4. Links & Media
Adding external resources and visuals.

* **Hyperlink:** `[Display Text](URL)`
* **Image:** `![Alt Text](ImageURL)`

> [!NOTE]
> Markdown does not natively support image resizing. Use HTML `<img>` tags if specific dimensions are required.

---

## 5. Code & Technical Snippets
Essential for developers to showcase syntax.

### Inline Code
Use single backticks: `` `npm install` ``

### Code Blocks
Use triple backticks with the language name for syntax highlighting.



```javascript
function greet() {
  console.log("Hello, World!");
}