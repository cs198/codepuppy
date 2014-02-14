angular.module('codepuppy').controller('AssignmentCtrl', function($scope, $routeParams) {
    $scope.$parent.selectedCourse = $routeParams.courseID;
    $scope.$parent.selectedAssignment = $routeParams.assignmentID;
    $scope.$parent.selectedSubmission = null;
    $scope.message = "I'm an assignment page for the course " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    var FAKEsubmissionsForGivenAssignment = ['rawatson_3', 'maesenc_5'];
    $scope.$parent.submissions = FAKEsubmissionsForGivenAssignment;
});
