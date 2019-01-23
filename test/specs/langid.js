'use strict'
const test = require('tape')
const path = require('path')
const fastText = require('../../index.js')

test('fastText language identification', function(t) {
  t.plan(3)

  let model_path = path.resolve(path.join(__dirname, '../models/lid.176.ftz'))
  console.log('File model path: ' + model_path)
  let classifier = new fastText.Classifier(model_path)

  Promise.all([
    classifier.predict('bonjour comment ça va', 5),
    classifier.predict('hello how are you?', 5),
    classifier.predict('como esta?', 5),
    classifier.predict('sử dụng vntk với fastext rất tuyệt?', 5)
  ])
    .then(res => {
      t.equal(res.length, 4, 'got 4 predictions')
      t.equal(res[0][0].label, '__label__fr', 'output is french')
      t.true(res[0][0].value > 0.9, 'confidence is > 90%')
    })
    .catch(err => t.fail(err))
})
