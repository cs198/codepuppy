angular.module('codepuppy').controller('MainCtrl', function($scope) {
    // create a message to display in our view
    $scope.message = 'Welcome screen for CodePuppy.  Select your class...';
    var FAKEclasses = ['cs106a', 'cs106b', 'cs106x', 'cs142'];
    $scope.$parent.classes = FAKEclasses;
    $scope.$parent.assignments = null;
});
