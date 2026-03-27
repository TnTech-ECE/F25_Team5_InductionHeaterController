import path from "path";
import fs from "fs";
function isFile(filePath) {
	try {
		const test = fs.lstatSync(filePath);
		return test.isFile();
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

// /**
//  * @type {number[]}
//  */
// const referencesToShiftFrom = [...(file.matchAll(/\[(\d+)#\]/g) ?? [])].map(match => Number(match[1])).reduce((s, c) => {
// 	if (s.includes(c)) return s;
// 	s.push(c);
// 	return s;
// }, []);
//if (referenceToShiftFromArray.length > 1) throw new Error("Multiple references to shift found");


const { base } = path.parse(filePath) ?? {};

const outPath = path.join(outFileFolder, base);
/**
 * @type {[number, string][]}
 */
const referencesToShift = [...(referencesFile.matchAll(/\[(\d+)#?\]/g) ?? [])].map(match => [Number(match[1]), match[0]]);
if (!referencesToShift || !referencesToShift.length) throw new Error("No reference to shift found");

referencesToShift.reverse();

const testReferencesToShiftObject = {};
/**
 * @type {string[]}
 */
const throws = [];
referencesToShift.forEach(([referenceToShift, referenceString], i) => {
	if (testReferencesToShiftObject[referenceString] !== undefined) {
		throws.push(`duplicate reference ${referenceString} at reference position: ${referencesToShift.length - i} and ${referencesToShift.length - testReferencesToShiftObject[referenceString]}`);
		return;
	}
	testReferencesToShiftObject[referenceString] = i;
});
if (throws.length) {
	throw new Error(throws.join('\n'));
}
// referencesToShift.forEach(([referenceToShift, referenceString], i) => {
// 	// console.warn({ isNewRef: referenceString.includes('#'), count, referenceString, newVal: maxReference - i });
// 	if (referenceString.includes('#')) return;
// 	file = file.replaceAll(`[${referenceToShift}]`, `[${referencesToShift.length - i}]`);
// });
const appendKey = "#28893983";
console.warn({ referencesToShift });
referencesToShift.forEach(([referenceToShift, referenceString], i) => {
	console.warn({ isNewRef: referenceString.includes('#'), i, referenceString, newVal: referencesToShift.length - i });
	file = file.replaceAll(`${referenceString}`, `[{${referencesToShift.length - i}${appendKey}}]`);
});
file = file.replaceAll(new RegExp(`\\[\\{(\\d+)${appendKey}\\}\\]`, "g"), (a, g1) => `[${g1}]`);
fs.writeFileSync(outPath, file);