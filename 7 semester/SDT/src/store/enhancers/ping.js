export const ping = store => next => action => {
    console.log(
        `Тип события: ${action.type},`,
        action.payload === undefined ? '' :
            `дополнительные данные события: ${JSON.stringify(action.payload)},`,
        `время возникновения события: ${new Date().toTimeString()}`);
    return next(action);
};
