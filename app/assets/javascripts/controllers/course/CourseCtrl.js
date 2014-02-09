angular.module('codepuppy').controller('CourseCtrl',
    function($scope, $routeParams) {
    $scope.message = 'Look! I am a class page for ' + $routeParams.courseID;
    $scope.$parent.selectedCourse = $routeParams.courseID;
    $scope.$parent.selectedAssignment = null;
    var FAKEassignmentsForGivenCourse = ['One', 'Two'];
    $scope.$parent.assignments = FAKEassignmentsForGivenCourse;
});
