module.exports = {
    temperature: 0,
    getData(req, res, next) {
        try {
            res.status(200).send({
                message: "Temperatura este : " + this.temperature
            });
        } catch (err) {
            res.status(500).send({
                error: 'Internal error.'
            })
        }
    },
    receiveData(req, res, next) {
        try {
            const temperature = req.body.temperature;
            this.temperature = temperature;

            console.log("Temperatura este : " + temperature);

            res.status(200).send({
                message: 'Data received.'
            });
        } catch (err) {
            res.status(500).send({
                error: 'Internal error.'
            })
        }
    }
}