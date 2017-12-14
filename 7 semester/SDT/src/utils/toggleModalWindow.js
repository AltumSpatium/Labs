function toggleModalWindow(windowName, itemName=null, itemId=null) {
    const windowState = this.state[windowName];
    const newState = {
        [windowName]: !windowState
    };

    if (itemName) newState[itemName] = itemId;
    this.setState(newState);
}

export default toggleModalWindow;
