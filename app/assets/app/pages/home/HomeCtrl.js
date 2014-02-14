angular.module('codepuppy').controller('HomeCtrl', function($scope) {
    // create a message to display in our view
    $scope.message = 'Welcome screen for CodePuppy.  Select your course...';
    $scope.$parent.assignments = null;
    $scope.$parent.selectedCourse = null;
    $scope.$parent.selectedAssignment = null;
});
