'use strict'
const test = require('tape')
const path = require('path')
const fastText = require('../../built')

test('fastText trainer', function(t) {
  t.plan(1)

  const data = path.resolve(path.join(__dirname, '../data/cooking.train.txt'))
  const model = path.resolve(path.join(__dirname, '../data/cooking.model'))
  const classifier = new fastText.Classifier()

  classifier
    .train('supervised', {
      input: data,
      output: model,
      loss: 'softmax',
      dim: 200,
      bucket: 2000000,
      verbose: 0
    })
    .then(res => {
      console.log('model info after training:', res)
      t.equal(res.dim, 200, 'dim')
    })
})

test('fastText quantize', function(t) {
  t.plan(1)
  const input = path.resolve(path.join(__dirname, '../data/cooking.train.txt'))
  const output = path.resolve(path.join(__dirname, '../data/cooking.model'))
  const classifier = new fastText.Classifier()

  classifier
    .train('quantize', {
      input,
      output,
      epoch: 1,
      qnorm: true,
      qout: true,
      retrain: true,
      cutoff: 1000,
      verbose: 0
    })
    .then(res => {
      console.log(res)
    })
    .catch(e => {
      console.error(e)
    })

  t.ok(true)
})
