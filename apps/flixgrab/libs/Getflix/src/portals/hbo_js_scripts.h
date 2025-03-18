#pragma once

constexpr const auto JS_SCRIPT_AUTO_SELECT_PROFILE = R"(
(() => {
    let output = "";

    let isprofileFrame = false;

    const walkTheDOM = (node) => {
    node = node.firstChild;
    while (node) {
        if (node.tagName == "SPAN" && node.textContent !== null) {
        /*console.log("  span: " + node.textContent);*/
        if (
            node.textContent.includes("Who") &&
            node.textContent.includes("Watching")
        ) {
            isprofileFrame = true;
            /*console.log("   in profile frame");*/
        } else if (isprofileFrame && node.textContent == "%1")
            return node;
        }
        const foundNode = walkTheDOM(node);
        if (foundNode !== null) return foundNode;
        node = node.nextSibling;
    }
    return null;
    };

    const viewport = document.getElementById("Viewport");
    if (viewport == null) return;
    /*console.log(" profile search...");*/
    try {
    const prof = walkTheDOM(viewport);
    if (prof !== null) {
        const profRect = prof.getBoundingClientRect();
        //console.log(' the interesting button found in: ' + profRect.left + 'x' + profRect.top);
        output = `{ "left": ${parseInt(profRect.left)}, "top": ${parseInt(profRect.top)}, "width": ${parseInt(profRect.width)}, "height": ${parseInt(profRect.height)} }`;
        console.log(" profile found: " + output);
    } else console.log(" not found profile.");
    } catch (err) {
    console.log("  error in profile search: " + err.message);
    }

    return output;
})();
)";


constexpr const auto JS_SCRIPT_AUTO_PLAY = R"(
(() => {
    let output = "";
    if (document.getElementById("Viewport") == null) return output;

    /*console.log(" button play search...");*/

    let found = false;

    const urls = [
    "/html/body/div[1]/div[4]/div[3]/div/div[1]/div/div/div[2]/div[1]",
    "/html/body/div[1]/div[4]/div[3]/div/div[1]/div/div/div[2]/a[1]/div"
    ];
                      
    for (const url of urls) {
    const element = document.evaluate(url, document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
    if (element !== null) {
        let btn = element.getBoundingClientRect();
        output = `{ "left": ${btn.left}, "top": ${btn.top} }`;
        found = true;
        console.log(" button found in: " + btn.left + "x" + btn.top);
        break;
    }
    }

    if (!found) console.log("not found button");

    return output;
})()
)";


constexpr const auto JS_SCRIPT_FIND_PROFILE = R"(
(() => {
  let element = document.evaluate("%1", document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
  if (element) {
    if ('textContent' in element) {
      console.log("Profile found: " + element.textContent);
      return element.textContent;
    }
  }
  return "";
})()
)";


constexpr const auto JS_SCRIPT_FIND_BY_XPATH = R"(
(() => {
    let element = document.evaluate("%1", document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
    if (element) {
        let rect = element.getBoundingClientRect();
        console.log("   >>> RECT <<<   ", rect);
        return `{ "left": ${rect.left}, "top": ${rect.top}, "width": ${rect.width}, "height": ${rect.height} }`;
    }
    return '{ "left": -1, "top": -1 }';
})()
)";


constexpr const auto JS_SCRIPT_FIND_INPUT_BY_PLACEHOLDER = R"(
(() => {
    let inputs = document.querySelectorAll("input");
    if (inputs !== null) {
        /*console.log(" found inputs: " + inputs.length);*/
        for (let i = 0; i < inputs.length; ++i) {
            let placeholder = inputs[i].getAttribute("placeholder");
            if (placeholder !== null && placeholder.includes("%1")) {
                inputs[i].value = '%2';
                return true;
            }
        }
    }
    return false;
})()
)";
