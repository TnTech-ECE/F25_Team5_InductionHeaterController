import path from "path";
import fs from "fs";
function isFile(filePath) {
	try {
		const test = fs.lstatSync(filePath);
		return test.isFile;
	} catch { }
	return false;
}
const [exe, jsFile, filePathText] = process.argv;
//"../../Reports/Conceptual Design.md"
const filePath = path.resolve(filePathText);
if (!isFile(filePath)) throw new Error("Path entered doesn't exist");
const outFileFolder = path.resolve("./out/");
let file = fs.readFileSync(filePath, "utf-8");
/**
 * @type {number[]}
 */
const referencesToShiftFrom = [...(file.matchAll(/\[(\d+)#\]/g) ?? [])].map(match => Number(match[1])).reduce((s, c) => {
	if (s.includes(c)) return s;
	s.push(c);
	return s;
}, []).sort((a, b) => a - b);
console.warn(filePathText, filePath, referencesToShiftFrom);
if (!referencesToShiftFrom || !referencesToShiftFrom.length) throw new Error("No reference to shift found");
//if (referenceToShiftFromArray.length > 1) throw new Error("Multiple references to shift found");

const minReferenceToShiftFrom = Math.min(...referencesToShiftFrom);

const { base } = path.parse(filePath) ?? {};

const outPath = path.join(outFileFolder, base);
// console.warn(base, outFileFolder, outPath);
/**
 * @type {number[]}
 */
const referencesToShift = [...(file.matchAll(/\[(\d+)\]/g) ?? [])].map(match => Number(match[1])).filter(d => d >= minReferenceToShiftFrom).reduce((s, c) => {
	if (s.includes(c)) return s;
	s.push(c);
	return s;
}, []).sort((a, b) => b - a);

let maxReference = Math.max(...referencesToShift);
maxReference = Math.max(maxReference, ...referencesToShiftFrom);

console.warn({ minReferenceToShiftFrom, maxReference, referencesToShiftFrom, referencesToShift });
let count = referencesToShiftFrom.length;
referencesToShift.forEach(referenceToShift => {
	console.warn(referenceToShift);
	file = file.replaceAll(`[${referenceToShift}]`, `[${referenceToShift + count}]`);
});

referencesToShiftFrom.forEach(reference => {
	file = file.replaceAll(`[${reference}#]`, `[${reference}]`);
});
fs.writeFileSync(outPath, file);