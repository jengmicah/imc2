var express = require('express')
var router = express.Router()
var registry_controller = require('../controllers/registry');

router.get('/medications', registry_controller.handle_get_medications);
router.get('/medications/:id', registry_controller.handle_get_medication);
router.post('/medications', registry_controller.handle_add_medication)
router.post('/medications/:id/events', registry_controller.handle_log_event);

module.exports=router;