import path from "path";
import fs from "fs";
function isFile(filePath) {
	try {
		const test = fs.lstatSync(filePath);
		return test.isFile;
	} catch { }
	return false;
}
const [exe, filePathText] = process.argv;
//"../../Reports/Conceptual Design.md"
const filePath = path.resolve(filePathText);
if (!isFile(filePath)) throw new Error("Path entered doesn't exist");
const outFileFolder = path.resolve("./out/");
let file = fs.readFileSync(filePath, "utf-8");

const referenceToShiftFromArray = [...(file.matchAll(/\[(\d+)#\]/g) ?? [])].map(match => Number(match[1]));

if (!referenceToShiftFromArray || !referenceToShiftFromArray.length) throw new Error("No reference to shift found");
if (referenceToShiftFromArray.length > 1) throw new Error("Multiple references to shift found");

const referenceToShiftFrom = referenceToShiftFromArray[0];
const { base } = path.parse(filePath) ?? {};

const outPath = path.join(outFileFolder, base);
// console.warn(base, outFileFolder, outPath);

const referencesToShift = [...(file.matchAll(/\[(\d+)\]/g) ?? [])].map(match => Number(match[1])).filter(d => d >= referenceToShiftFrom);
console.warn(referenceToShiftFrom, referencesToShift);
for (let i = referencesToShift.length - 1; i >= 0; i--) {
	const referenceToShift = referencesToShift[i];
	file = file.replaceAll(`[${referenceToShift}]`, `[${referenceToShift + 1}]`);
}
file = file.replaceAll(`[${referenceToShiftFrom}#]`, `[${referenceToShiftFrom}]`);
fs.writeFileSync(outPath, file);