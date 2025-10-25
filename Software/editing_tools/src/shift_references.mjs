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
const referencesIndex = file.search(/[#]+\s+References/g);
const referencesFile = file.slice(referencesIndex, file.length - 1);
//console.warn({ referencesIndex, referencesFile });

/**
 * @type {number[]}
 */
const referencesToShiftFrom = [...(file.matchAll(/\[(\d+)#\]/g) ?? [])].map(match => Number(match[1])).reduce((s, c) => {
	if (s.includes(c)) return s;
	s.push(c);
	return s;
}, []);
console.warn(filePathText, filePath, referencesToShiftFrom);
if (!referencesToShiftFrom || !referencesToShiftFrom.length) throw new Error("No reference to shift found");
//if (referenceToShiftFromArray.length > 1) throw new Error("Multiple references to shift found");

const minReferenceToShiftFrom = Math.min(...referencesToShiftFrom);

const { base } = path.parse(filePath) ?? {};

const outPath = path.join(outFileFolder, base);
// console.warn(base, outFileFolder, outPath);
/**
 * @type {[number, string][]}
 */
const referencesToShift = [...(referencesFile.matchAll(/\[(\d+)#?\]/g) ?? [])].map(match => [Number(match[1]), match[0]]);
referencesToShift.reverse();
let maxReference = referencesToShift.length;

console.warn({ minReferenceToShiftFrom, maxReference, referencesToShiftFrom, referencesToShift });
let count = referencesToShiftFrom.length;
referencesToShift.forEach(([referenceToShift, referenceString], i) => {
	console.warn({ isNewRef: referenceString.includes('#'), count, referenceString, newVal: maxReference - i });
	if (referenceString.includes('#')) return;
	file = file.replaceAll(`[${referenceToShift}]`, `[${maxReference - i}]`);
});
console.warn("wdkjwdjkwkjdjkwd\n\n\n");

referencesToShift.forEach(([referenceToShift, referenceString], i) => {

	if (!referenceString.includes('#')) return;
	console.warn({ isNewRef: referenceString.includes('#'), count, referenceString, newVal: maxReference - i });

	file = file.replaceAll(`[${referenceToShift}#]`, `[${maxReference - i}]`);
});
fs.writeFileSync(outPath, file);