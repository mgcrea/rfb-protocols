const addon = require('./../build/Release/addon');

var obj = new addon.Hextile(10, 10);
console.log(obj.plusOne()); // 11
console.log(obj.plusOne()); // 12
console.log(obj.plusOne()); // 13
