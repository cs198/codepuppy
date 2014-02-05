codepuppy.controller('mainController', function($scope) {
    // create a message to display in our view
    $scope.message = 'Welcome screen for CodePuppy.  Select your class...';
    $scope.$parent.classes = FAKEclasses;
    $scope.$parent.assignments = null;
});
