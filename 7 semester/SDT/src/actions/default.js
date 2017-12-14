export const request = type => () => ({type: type});
export const success = type => payload => ({type: type, payload: payload});
export const failure = (type, callback) => err => {
    callback(err);
    return { type: type };
};
