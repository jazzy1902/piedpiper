class Node {
    constructor(char, freq) {
      this.char = char;
      this.freq = freq;
      this.left = null;
      this.right = null;
    }
  }
  
  function buildHuffmanTree(text) {
    // Step 1: Calculate frequency of each character
    const freqMap = {};
    for (let char of text) {
      freqMap[char] = (freqMap[char] || 0) + 1;
    }
  
    // Step 2: Create a priority queue (min heap) of nodes
    const nodes = Object.entries(freqMap).map(([char, freq]) => new Node(char, freq));
    nodes.sort((a, b) => a.freq - b.freq);
  
    while (nodes.length > 1) {
      const left = nodes.shift();
      const right = nodes.shift();
      const parent = new Node(null, left.freq + right.freq);
      parent.left = left;
      parent.right = right;
      nodes.push(parent);
      nodes.sort((a, b) => a.freq - b.freq);
    }
  
    return nodes[0];
  }
  
  function generateCodes(root, code = "", codes = {}) {
    if (!root) return;
    if (root.char !== null) {
      codes[root.char] = code || "0";
    }
    generateCodes(root.left, code + "0", codes);
    generateCodes(root.right, code + "1", codes);
    return codes;
  }
  
  function huffmanCompress(text) {
    const tree = buildHuffmanTree(text);
    const codes = generateCodes(tree);
  
    // Encode the text
    let encoded = "";
    for (let char of text) {
      encoded += codes[char];
    }
  
    return { encoded, codes };
  }
  
  function huffmanDecompress(encoded, codes) {
    const reverseCodes = Object.fromEntries(
      Object.entries(codes).map(([char, code]) => [code, char])
    );
  
    let decoded = "";
    let currentCode = "";
    for (let bit of encoded) {
      currentCode += bit;
      if (reverseCodes[currentCode]) {
        decoded += reverseCodes[currentCode];
        currentCode = "";
      }
    }
  
    return decoded;
  }
  
  export { huffmanCompress, huffmanDecompress };