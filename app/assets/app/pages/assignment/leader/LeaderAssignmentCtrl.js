angular.module('codepuppy').controller('LeaderAssignmentCtrl', ['$scope',
    '$stateParams', '$fileUploader', '$http', '$sce',
    function($scope, $stateParams, $fileUploader, $http, $sce)
{
    var getAssignment = function() {
        $http({
            method: 'GET',
            url: '/assignments/' +
                 $stateParams.assignmentID + '.json'
        })
        .success(function(data, status, headers, config) {
            if (data.name) {
                $scope.assignmentName = data.name;
            } else {
                $scope.assignmentName = 'Assignment';
            }

            if (data.description) {
                if (iswiki(data.description)) {
                    $scope.assignmentDescription = $sce.trustAsHtml(
                        wiki2html(data.description)
                    );
                } else {
                    $scope.assignmentDescription = data.description;
                }
            } else {
                $scope.assignmentDescription = '';
            }
        });
    };
    getAssignment();
}]);
