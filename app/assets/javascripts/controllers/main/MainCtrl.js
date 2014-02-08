angular.module('codepuppy').controller('MainCtrl', function($scope) {
    // create a message to display in our view
    $scope.message = 'Welcome screen for CodePuppy.  Select your class...';
    $scope.$parent.assignments = null;
    $scope.$parent.selectedClass = null;
    $scope.$parent.selectedAssignment = null;
});
